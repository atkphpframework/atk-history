<?php
/**
* This file is part of the Ibuildings E-business Platform.
* Detailed copyright and licensing information can be found
* in the doc/COPYRIGHT and doc/LICENSE files which should be
* included in the distribution.
*
* @package ATK
*
* @author martin <martin@ibuildings.nl>
* @author peter <peter@ibuildings.nl>
*
* @copyright (c) 2006 Ibuildings.nl BV
* @license see doc/LICENSE
*
* @version $Revision$
* $Id$
*/

atkimport('atk.utils.atkdataholder');

/**
 * FrontEnd Controller base class.
 *
 * @author Martin Roest <martin@ibuildings.nl>
 * @author Peter C. Verhage <peter@ibuildings.nl>
 */
class atkFrontController implements ArrayAccess
{
  protected $m_module;
  protected $m_name;
  protected $m_action;
  protected $m_parent;

  protected $m_request;
  protected $m_session;

  protected $m_vars = array();
  protected $m_template = "";

  protected $m_plugins = NULL;

  protected $m_partial = false;
  protected $m_result = "";
  protected $m_rendered = false;

  protected $m_headers = array();
  protected $m_contentType;

  /**
   * Create controller based on the given URI.
   *
   * @param string $uri uri string
   * @param atkFrontController $parent parent controller
   * @return atkFrontController the controller
   */
  public static function create($uri, $parent=NULL)
  {
    if ($uri{0} == '/')
      $uri = substr($uri, 1);

    list($module, $name, $action) = explode('/', $uri);

    if ($action == NULL)
      $action = 'index';

    $importPath = "module.{$module}.controllers.{$name}controller";
    if (atkimport($importPath))
    {
      $class = "{$name}controller";
      return new $class($module, $name, $action, $parent);
    }
    else
    {
      throw new Exception("Can't find controller for: '$uri' expecting '$importpath'");
    }
  }

  /**
   * Returns the current controller instance. This can either be the
   * root or a nested controller.
   *
   * @return atkFrontController current controller instance
   */
  public static function getCurrent()
  {
    $smarty = atkSmarty::getInstance();
    return $smarty->__controller;
  }

  /**
   * Constructor.
   *
   * @param string $module module name
   * @param string $name   controller name
   */
  protected function __construct($module, $name, $action, $parent = NULL)
  {
    $this->m_module = $module;
    $this->m_name = $name;
    $this->m_action = $action;
    $this->m_parent = $parent;
  }

  /**
   * Returns the parent controller.
   *
   * @return atkFrontController parent
   */
  public function getParent()
  {
    return $this->m_parent;
  }

  /**
   * Is this the root controller?
   *
   * @return boolean is root controller?
   */
  public function isRoot()
  {
    return $this->getParent() == NULL;
  }

  /**
   * Returns the root controller.
   *
   * @return atkFrontController root
   */
  public function getRoot()
  {
    if ($this->isRoot()) return $this;
    else return $this->getParent()->getRoot();
  }

  /**
   * Returns the request parameters.
   *
   * @return atkDataHolder request parameters
   */
  public function getRequest()
  {
    return $this->m_request;
  }

  /**
   * Returns the session.
   *
   * @return atkDataHolder session
   */
  public function getSession()
  {
    return $this->m_session;
  }

  /**
   * Returns the controller name.
   *
   * @return string controller name
   */
  public function getName()
  {
    return $this->m_name;
  }

  /**
   * Returns the controller module.
   *
   * @return string controller module
   */
  public function getModule()
  {
    return $this->m_module;
  }

  /**
   * Returns the action.
   *
   * @return string action
   */
  public function getAction()
  {
    return $this->m_action;
  }

  /**
   * Add response header. (Will only be outputted when
   * renderPartial or renderContent is called for the
   * root controller).
   */
  protected function addHeader($header)
  {
    $this->m_headers[] = $header;
  }

  /**
   * Sets the content type (Will only be outputted when
   * renderPartial or renderContent is called for the
   * root controller).
   *
   * @param string $contentType
   */
  protected function setContentType($contentType)
  {
    $this->m_contentType = $contentType;
  }

  /**
  * Handle action.
  *
  * @param array $request request variables
  *
  * @return result of action
  */
  public function handleRequest($request)
  {
    try
    {
      try 
      {
        $method = $this->getActionMethod();
      }
      catch (Exception $ex)
      {
        throw new Exception("Invalid action {$this->m_action} for controller {$this->m_module}/{$this->m_name}! (".$ex->getMessage().")");
      }
      
      $this->setTemplate($this->m_action);

      $this->m_request = new atkDataHolder($request);
      $this->m_session = $this->loadSession();

      $this->module =   $this->m_module;
      $this->name =     $this->m_name;
      $this->action =   $this->m_action;
      $this->request =  $this->m_request;
      $this->session =  $this->m_session;

      $this->installPlugins();

      $this->preFilter();
      $method->invoke($this);
      $this->render();
      $this->postFilter();

      $this->uninstallPlugins();

      if ($this->m_partial && $this->isRoot())
      {
        while (@ob_end_clean());

        foreach ($this->m_headers as $header)
          header($header);

        if (isset($this->m_contentType))
          header('Content-Type: '.$this->m_contentType);

        $output = atkinstance('atk.ui.atkoutput');
        $output->output($this->m_result);
        $output->outputFlush();
        

        die;
      }
    }
    catch (Exception $ex)
    {
      $this->uninstallPlugins();
      $this->handleException($ex);
    }

    return $this->m_result;
  }

  /**
   * Get the method to call for the current request.
   *
   * @return ReflectionMethod The method.
   */
  public function getActionMethod()
  {
    $methodName = str_replace('_', '', $this->m_action).'Action';
    if (!method_exists($this, $methodName))
      throw new Exception("Action method not found!");

    $method = new ReflectionMethod(get_class($this), $methodName);
    if (!$method->isPublic())
      throw new Exception("Action method is not public!");
      
    return $method;
  }

  /**
   * Will be called just before the request is dispatched
   * to the right method.
   */
  protected function preFilter()
  {}

  /**
   * Will be called just after the request has been dispatched
   * to the right method and the rendering took place but before
   * the result is returned or outputted.
   */
  protected function postFilter()
  {}

  /**
   * Handle exception.
   *
   * @param $exception exception
   */
  protected function handleException($exception)
  {
    if ($this->isRoot())
    {
      atkerror($exception->__toString());
      $this->renderContent('An unknown error occured.');
    }
    else
    {
      throw $exception;
    }
  }

  /**
   * Load session.
   */
  protected function loadSession()
  {
    return new atkDataHolder($_SESSION);
  }

  /**
   * Install smarty plug-ins.
   */
  protected function installPlugins()
  {
    /* @var $smarty Smarty */
    $smarty = atkinstance("atk.ui.atksmarty");
    $this->m_plugins = $smarty->_plugins;
    $smarty->register_function('partial', array($this, '__smarty_function_partial'), false);    
    $smarty->register_function('url', array($this, '__smarty_function_url'), false);
    $smarty->register_function('form_vars', array($this, '__smarty_function_form_vars'), false);    
    $smarty->register_block('link', array($this, '__smarty_block_link'), false);
  }

  /**
   * Uninstall smarty plug-ins.
   */
  protected function uninstallPlugins()
  {
    if ($this->m_plugins == NULL) return;
    $smarty = atkinstance("atk.ui.atksmarty");
    $smarty->_plugins = $this->m_plugins;
    $this->m_plugins = NULL;
  }

  /**
   * Get current template.
   *
   * @return string $template template name
   */
  protected function getTemplate()
  {
    return $this->m_template;
  }

  /**
   * Set current template.
   *
   * @param string $template template name
   */
  protected function setTemplate($template)
  {
    $this->m_template = $template;
  }

  /**
   * Get assigned template variables.
   *
   * @return array template variables
   */
  protected function getVars()
  {
    return $this->m_vars;
  }

  /**
   * Is template variable set?
   */
  public function __isset($name)
  {
    return isset($this->m_vars[$name]);
  }

  /**
   * Template variable assignment.
   *
   * @param string $name variable name
   * @param unknown $value variable value
   */
  public function __set($name, $value)
  {
    $this->m_vars[$name] = $value;
  }

  /**
   * Get template variable value.
   *
   * @param string $name variable name
   * @return unknown
   */
  public function &__get($name)
  {
    return $this->m_vars[$name];
  }

  /**
   * Checks if a certain offset exists.
   * 
   * @param mixed $offset offset
   * @return bool offset exists?
   */
  public function offsetExists($offset)
  {
    return isset($this->m_vars[$offset]);
  }
  
  /**
   * Returns the value at the given offset.
   *
   * @param mixed $offset
   * @return mixed value
   */
  public function offsetGet($offset)
  {
    return $this->m_vars[$offset];
  }
  
  /**
   * Sets the given offset with the given value
   *
   * @param mixed $offset offset
   * @param mixed $value  value
   */  
  public function offsetSet($offset, $value)
  {
    $this->m_vars[$offset] = $value;
  }
  
  /**
   * Unsets the given offset.
   *
   * @param mixed $offset offset
   */
  public function offsetUnset($offset)
  {
    unset($this->m_vars[$offset]);
  }
  
  /**
   * Build the URI for the given controller and action.
   *
   * @param string $controller controller name (uses current if empty)
   * @param string $action action name (uses current if empty and controller isn't changed, uses index if empty and other controller)
   * @return string uri
   */
  protected function uri($controller, $action)
  {
    if ($this->isRoot())
    {
      $controller = str_replace('.', '/', $controller);

      if ($controller != NULL && strpos($controller, '/') === FALSE)
      {
        $controller = "{$this->m_module}/{$controller}";
      }

      if ($controller == NULL || $controller == "{$this->m_module}/{$this->m_name}")
      {
        $controller = "{$this->m_module}/{$this->m_name}";

        if ($action == NULL)
          $action = $this->m_action;
      }
      else if ($action == NULL)
      {
        $action = 'index';
      }

      return '/'.$controller.'/'.$action;
    }
    else
    {
      return $this->getParent()->uri($controller, $action);
    }
  }
  
  /**
   * Build url or return the current url.
   *
   * @param string $controller controller name (uses current if empty)
   * @param string $action action name (uses current if empty and controller isn't changed, uses index if empty and other controller)
   * @param array $vars request vars
   * @return string url
   */
  public function url($controller=NULL, $action=NULL, $vars=array())
  {
    $uri = $this->uri($controller, $action);
    $vars['uri'] = $uri;
    $url = $_SERVER['PHP_SELF'].'?'.http_build_query($vars);
    return $url;
  }  
  
  /**
   * Smarty wrapper function for the url function.
   *
   * @param array $params parameters
   * @param Smarty $smarty smarty reference
   * @return string url
   */
  public function __smarty_function_url($params, $smarty)
  {
    $controller = isset($params['controller']) ? $params['controller'] : NULL;
    $action = isset($params['action']) ? $params['action'] : NULL;
    $vars = isset($params['vars']) ? $params['vars'] : array();
    
    unset($params['controller']);
    unset($params['action']);
    unset($params['vars']);

    $vars = array_merge($vars, $params);
    
    return $this->url($controller, $action, $vars);
  }  

  /**
   * Smarty function for converting the controller, action etc.
   * to hidden form vars.
   *
   * @param array $params parameters
   * @param Smarty $smarty smarty reference
   * @return string hidden form vars html
   */
  public function __smarty_function_form_vars($params, $smarty)
  {
    $controller = isset($params['controller']) ? $params['controller'] : NULL;
    $action = isset($params['action']) ? $params['action'] : NULL;
    $vars = isset($params['vars']) ? $params['vars'] : array();

    unset($params['controller']);
    unset($params['action']);
    unset($params['vars']);

    $uri = $this->uri($controller, $action);
    $vars = array_merge($vars, $params, array('uri' => $uri));

    $result = '';
    foreach ($vars as $name => $value)
    {
      $value = htmlentities($value);
      $result .= "<input type=\"hidden\" name=\"{$name}\" value=\"{$value}\">\n";
    }

    return $result;
  }
  
  /**
   * Smarty wrapper function for the url function.
   *
   * @param array $params parameters
   * @param string $content content between link tags
   * @param Smarty $smarty smarty reference
   * @param boolean $repeat true first time
   */
  public function __smarty_block_link($params, $content, $smarty, &$repeat)
  {
     if (!$repeat)
    {
      return '<a href="'.$this->__smarty_function_url($params, $smarty).'">'.$content.'</a>';
    }
    
    return NULL;
  }  

  /**
   * Smarty partial function, render partial template.
   *
   * @param $params parameters
   * @param Smarty $smarty smarty instance
   * @return string rendered partial
   */
  public function __smarty_function_partial($params, $smarty)
  {
    $template = $params[0];
    unset($params[0]);
    return $this->_render($template, $params, true);
  }

  /**
  * Set locale based on the given language.
  *
  * @param string $lng language (ISO code)
  */
  protected function setLocale($lng)
  {
    switch (strtoupper($lng))
    {
      case "EN":
        setlocale(LC_ALL, "en_EN");
        break;
      case "DE":
        setlocale(LC_ALL, "de_DE");
        break;
      case "FR":
        setlocale(LC_ALL, "fr_FR");
        break;
      default:
        setlocale(LC_ALL, "nl_NL");
    }
  }

  /**
   * Render partial.
   *
   * Will assign the partial and result variables and will prevent
   * double rendering!
   *
   * @param string $template template name
   * @param array $vars template variables
   */
  protected function renderPartial($template=NULL, $vars=NULL)
  {
    $this->render($template, $vars, true);
  }

  /**
   * Render content.
   *
   * Will assign the partial and result variables and will prevent
   * double rendering!
   *
   * @param string $content content to render
   */
  protected function renderContent($content)
  {
    $this->m_partial = true;
    $this->m_result = $content;
    $this->m_rendered = true;
  }

  /**
   * Render template.
   *
   * Will assign the partial and result variables and will prevent
   * double rendering!
   *
   * @param string $template template name
   * @param array $vars template variables
   * @param boolean $partial render partial?
   */
  public function render($template=NULL, $vars=NULL, $partial=false)
  {
    if ($this->m_rendered) return;

    if ($template == NULL)
      $template = $this->getTemplate();

    if ($vars == NULL)
      $vars = $this->getVars();

    if (!$partial && file_exists(moduleDir($this->m_module)."scripts/{$this->m_name}.js"))
    {
      $this->registerScriptFile(moduleDir($this->m_module)."scripts/{$this->m_name}.js");
    }
    
    if (!$partial && file_exists(moduleDir($this->m_module)."styles/{$this->m_name}.js"))
    {
      $this->registerStyleSheet(moduleDir($this->m_module)."styles/{$this->m_name}.js");
    }
    
    $this->m_partial = $partial;
    $this->m_result = $this->_render($template, $vars, $partial);
    $this->m_rendered = true;
  }

  /**
   * Register stylesheet of the given media type.
   *
   * @param string $file stylesheet filename
   * @param string $media media type (defaults to 'all')
   */
  protected function registerStyleSheet($file,$media='all')
  {
    atkinstance('atk.ui.atkpage')->register_style($file,$media);
  }

  /**
   * Register stylesheet code.
   *
   * @param string $code stylesheet code
   */
  protected function registerStyleCode($code)
  {
    atkinstance('atk.ui.atkpage')->register_stylecode($code);
  }

  /**
   * Register script file.
   *
   * @param string $file script filename
   */
  protected function registerScriptFile($file)
  {
    atkinstance('atk.ui.atkpage')->register_script($file);
  }
  
  /**
   * Register JavaScript code.
   *
   * @param string $code
   */
  protected function registerScriptCode($code)
  {
    atkinstance('atk.ui.atkpage')->register_scriptcode($code);
  }

  /**
   * Returns the template path for the given template.
   *
   * @param string $template  template name
   * @param string $directory the base template directory
   * 
   * @return string template path
   */
  protected function getTemplatePath($template, $directory='templates')
  {
    if (file_exists(atkconfig('atkroot')."$directory/$template"))
    {
      return atkconfig('atkroot')."$directory/$template";
    }
    else if (file_exists(moduleDir($this->m_module)."$directory/$template"))
    {
      return moduleDir($this->m_module)."$directory/$template";
    }
    else if (file_exists(moduleDir($this->m_module)."skel/$directory/$template"))
    {
      return moduleDir($this->m_module)."skel/$directory/$template";
    }
    else 
    {
      return null;
    }
  }
  
  /**
   * Returns the layout template path for this controller.
   */
  protected function getLayoutTemplatePath()
  {
    $path = $this->getTemplatePath($this->m_module.'/'.$this->m_name.'.tpl', 'layouts');    
    if ($path != null) return $path;
    
    $path = $this->getTemplatePath($this->m_module.'/root.tpl', 'layouts');
    if ($path != null) return $path;
    
    $path = $this->getTemplatePath('root.tpl', 'layouts');
    if ($path != null) return $path;    
    
    return $null;
  }

  /**
   * Render template.
   *
   * @param string $template template name
   * @param array $vars template variables
   * @param boolean $partial render partial?
   *
   * @return string rendered template
   */
  protected function _render($template, $vars, $partial)
  {
    if ($template{0} != '/')
    {
      $template = $this->m_module.'/'.$this->m_name.'/'.($partial ? '_' : '').$template;
    }
    else
    {
      $parts = explode('/', $template);
      $template = implode('/', array_slice($parts, 0, count($parts) - 1)).'/'.($partial ? '_' : '').$parts[count($parts) - 1];
    }

    $template = "{$template}.tpl";

    /* @var $smarty Smarty */
    $smarty = atkinstance("atk.ui.atksmarty");
    $oldVars = $smarty->get_template_vars();
    $smarty->assign($vars);
    $oldCaching = $smarty->caching;
    $smarty->caching = false;
    $oldForceCompile = $smarty->force_compile;
    $smarty->force_compile = true;
    $result = $smarty->fetch($this->getTemplatePath($template));
    
    $layoutTemplatePath = $this->getLayoutTemplatePath();
    if ($this->isRoot() && !$partial && $layoutTemplatePath != null)
    {
      $vars['content_for_layout'] = $result;
      $smarty->assign($vars);
      $result = $smarty->fetch($layoutTemplatePath);
    }
    
    $smarty->force_compile = $oldForceCompile;
    $smarty->caching = $oldCaching;
    $smarty->assign($oldVars);
    return $result;
  }

  /**
   * To string.
   */
  public function __toString()
  {
    return (!$this->isRoot() ? $this->getParent()->__toString().'/' : '').$this->getName();
  }
}

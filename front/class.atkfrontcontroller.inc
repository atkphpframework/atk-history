<?php
/**
* This file is part of the Ibuildings E-business Platform.
* Detailed copyright and licensing information can be found
* in the doc/COPYRIGHT and doc/LICENSE files which should be
* included in the distribution.
*
* @package ATK
*
* @author Peter C. Verhage <peter@ibuildings.nl>
*
* @copyright (c) 2007 Ibuildings.nl BV
* @license see doc/LICENSE
*
* @version $Revision$
* $Id$
*/

atkimport('atk.front.atkfronttools');
atkimport('atk.front.routing.atkurlrewriter');
atkimport('atk.front.views.atkview');

/**
 * Front-end controller base class.
 */
class atkFrontController implements ArrayAccess
{
  const DEFAULT_STATUS = 200;
  
  /**
   * Current controller.
   *
   * @var atkFrontController
   */
  private static $s_current = null;

  /**
   * Controller bridge.
   * 
   * @var atkFrontControllerBridge
   */
  private $m_bridge;
  
  /**
   * Controller view.
   * 
   * @var atkView
   */
  private $m_view;
  
  /**
   * Controller name.
   *
   * @var string
   */
  private $m_name;

  /**
   * Controller package.
   * 
   * @var string
   */
  private $m_package;
  
  /**
   * Action name.
   * 
   * @var unknown_type
   */
  private $m_action;
  
  /**
   * Parent controller (optional!).
   *
   * @var atkFrontController
   */
  private $m_parent;

  /**
   * Template variables.
   *
   * @var array
   */
  private $m_vars = array();
  
  /**
   * Rendered?
   *
   * @var boolean
   */
  private $m_rendered = false;

  /**
   * URL rewriter.
   * 
   * @var atkUrlRewriter
   */
  private $m_url = null;
  
  /**
   * Request object. Can be accessed like an array for the
   * request parameters.
   * 
   * @var atkRequest
   */
  protected $request;
  
  /**
   * Request parameters object. Can be accessed like an array.
   * 
   * @var ArrayAccess
   */
  protected $params;

  /**
   * Session object. Can be accessed like an array.
   * 
   * @var atkSession
   */
  protected $session;
  
  /**
   * Response object.
   * 
   * @var atkResponse
   */
  protected $response;
  
  /**
   * Create controller instance for the controller with the given import path.
   * 
   * @param string $path ATK import path.
   * 
   * @return atkController controller instance
   */
  public static function create($path)
  {
    $class = substr($path, strrpos($path, '.') + 1);
    $name = substr($class, 0, -strlen('controller'));
    $package = substr($path, 0, strrpos($path, '.'));

    atkimport($path);
    return new $class($name, $package);
  }
  
  /**
   * Constructor.
   * 
   * @param string $name    controller name
   * @param string $package controller package
   */
  protected function __construct($name, $package)
  {
    $this->m_name = $name;
    $this->m_package = $package;
    $this->m_parent = atkFrontController::getCurrent();
    $this->m_bridge = atkFrontControllerBridge::getInstance();
    $this->m_view = atkView::create($this);
  }
  
  /**
   * Process the given request and return the result in the response object. 
   * For convenience the response object will also be returned.
   * 
   * @param atkRequest  $request  request object
   * @param atkResponse $response response object
   * @param string      $method   action handler method name
   * @param array       $params   parameters for the action handler method
   * 
   * @return atkResponse response object
   */
  public function process($request, $response, $method='performAction', $params=array())
  {
    $this->request = $request;
    $this->params = $request; // for now
    $this->response = $response;
    $this->session = $request->getSession();
    
    $this->m_action = $this->request['action'] ? $this->request['action'] : 'index';
    
    $this->m_url = new atkUrlRewriter($request);
    
    self::setCurrent($this);
    
    try
    {
      $this->init();
      $this->preFilter();
      call_user_func_array(array($this, $method), $params);
      $this->postFilter();      
    }
    catch (Exception $exception)
    {
      self::setCurrent($this->getParent());
      throw $exception;
    }
      
    self::setCurrent($this->getParent());
    
    return $response;
  }
  
  public function processWithException($request, $response, $exception)
  {
    return $this->process($request, $response, 'rescueAction', array($exception));
  }
  
  /**
   * Perform action.
   */
  protected function performAction()
  {
    $name = atkFrontTools::camelize($this->getAction()).'Action';

    // call action method if available and public
    if (method_exists($this, $name) && 
        ($method = new ReflectionMethod($this, $name)) &&
        $method->isPublic())
    {
      $method->invoke($this);
    }
    
    // let generic catch-all mechanism handle the action
    else if (method_exists($this, 'actionMissing'))
    {
      $this->actionMissing($this->getAction());
    }
    
    // if a template exists for the action, we render it anyway
    else if (!$this->getView()->templateExists())
    {
      throw new atkFrontException(atkFrontException::UNKNOWN_ACTION, "Unknown action '".$this->getAction()."' for controller '".$this->getFullName()."'");
    }
    
    // render (if not already rendered)
    if (!$this->isRendered())
    {
      $this->render();
    }
  }
  
  /**
   * Rescue action.
   * 
   * @param Exception $exception exception object
   */
  protected function rescueAction($exception)
  {
    if ($this->isLocalRequest())
    {
      $this->rescueActionLocally($exception);
    }
    else 
    {
      $this->rescueActionInPublic($exception);
    }
  }
  
  /**
   * Is this a local request? You can adjust this to add for example a 
   * developer's IP address. By default requests from 127.0.0.1 and 
   * requests with a debug level >= 2 a treated "locally".
   *
   * @return boolean is local request?
   */
  protected function isLocalRequest()
  {
    return $this->request->getRemoteAddr() == "127.0.0.1" || atkconfig('debug_level') >= 2;
  }
  
  /**
   * @param Exception $exception
   */
  protected function rescueActionLocally($exception)
  {
    $this['exception'] = $exception;
    $this['message'] = $exception->getMessage();    
    $this['trace'] = $exception->getTrace();
    $this->render(array('text' => '<b>Message:</b><br>'.$this['message'].'<br><br><b>Trace:</b><br>'.nl2br($exception->getTraceAsString()))); // TODO!
  }
  
  /**
   * Enter description here...
   *
   * @param Exception $exception
   */
  protected function rescueActionInPublic($exception)
  {
    if ($exception instanceof atkFrontException && $exception->isNotFound())
    {
      $this->render(array('text' => 'Page not found', 'status' => '404 Not Found'));
    }
    else 
    {
      $this->render(array('text' => 'Internal server error', 'status' => '501 Internal Server Error'));
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
    return self::$s_current;
  }

  /**
   * Sets the current controller instance.
   *
   * @param atkFrontController $controller current controller instance
   */
  private static function setCurrent($controller)
  {
    self::$s_current = $controller;
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
   * Returns the request object.
   *
   * @return atkRequest request object
   */
  public function getRequest()
  {
    return $this->request;
  }
  
  /**
   * Returns the response object.
   * 
   * @return atkResponse response object
   */
  public function getResponse()
  {
    return $this->response;
  }

  /**
   * Returns the session object.
   *
   * @return atkSession session object
   */
  public function getSession()
  {
    return $this->session;
  }
  
  /**
   * Returns the controller bridge.
   * 
   * @return atkFrontControllerBridge
   */
  public function getBridge()
  {
    return $this->m_bridge;
  }

  /**
   * Returns the controller view.
   * 
   * @return atkView
   */
  public function getView()
  {
    return $this->m_view;
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
   * Returns the controller package.
   * 
   * @return string controller package
   */
  public function getPackage()
  {
    return $this->m_package;
  }
  
  /**
   * Get package path for this controller.
   * 
   * @return string controller package path
   */
  public function getPackagePath()
  {
    return getClassPath($this->getPackage(), false).'/';
  }
  
  /**
   * Returns the module name for this controller if
   * this controller is part of a module.
   * 
   * @return string module name
   */
  public function getModule()
  {
    $package = $this->getPackage();
    $parts = explode('.', $package);
    return $parts[0] == 'module' && count($parts) > 1 ? $parts[1] : null;
  }
  
  /**
   * Returns the full controller name, including package(s).
   * 
   * @return string full controller name
   */
  public function getFullName()
  {
    return $this->getPackage().'.'.$this->getName();
  }

  /**
   * Returns the current action.
   *
   * @return string action
   */
  public function getAction()
  {
    return $this->m_action;
  }
  
  /**
   * Returns the helper instances for this controller
   * 
   * @return array helper instances
   */
  public function getHelpers()
  {
    static $helpers = null;
    if ($helpers == null)
    {
      atkimport('atk.front.helpers.atkhelper');
      $helpers = array();
      $helpers[] = atknew('atk.front.helpers.atkformhelper', $this);
      $helpers[] = atknew('atk.front.helpers.atki18nhelper', $this);    
      $helpers[] = atknew('atk.front.helpers.atkurlhelper', $this);          
      $helpers[] = atknew('atk.front.helpers.atkscripthelper', $this);
      $helpers[] = atknew('atk.front.helpers.atkstylehelper', $this);
    }
    return $helpers;
  }
  
  /**
   * Is rendered?
   * 
   * @return boolean is rendered?
   */
  protected function isRendered()
  {
    return $this->m_rendered;
  }
  
  /**
   * Is partial?
   * 
   * @return boolean is partial?
   */
  protected function isPartial()
  {
    return $this->m_partial;
  }

  /**
   * Will be called just before the request is dispatched
   * to the right method.
   */
  protected function preFilter()
  {}

  /**
   * Init will be called just after preFilter and just before
   * the request is dispatched to the right method.
   */
  protected function init()
  {}

  /**
   * Will be called just after the request has been dispatched
   * to the right method and the rendering took place but before
   * the result is returned or outputted.
   */
  protected function postFilter()
  {}

  /**
   * Set wether or not the controller has rendered yet
   *
   * @param bool $rendered
   */
  protected function setRendered($rendered)
  {
    $this->m_rendered = $rendered;
  }

  /**
   * Get assigned template variables.
   *
   * @return array template variables
   */
  public function getVars()
  {
    return $this->m_vars;
  }

  /**
   * Returns an URL that has been rewritten according to given parameters
   * and the defined routes. The following parameters are treated specially:
   * 
   * - 'anchor'           Ñ Specifies the anchor name to be appended to the path.
   * - 'only_path'        Ñ If true, returns the relative URL (omitting the 
   *                        protocol, host name, and port) (false by default).
   * - 'trailing_slash'   - If true, adds a trailing slash, as in 
   *                        "/archive/2005/" (false by default).
   * - 'host'             - Overrides the default (current) host if provided.
   * - 'protocol'         - Overrides the default (current) protocol if provided.
   * - 'port'             - Overrides the default port to connect to.
   * - 'overwrite_params' - See the description below.
   * 
   * The URL is generated from the remaining parameters. An URL contains two 
   * key parts: the <base> and a query string. atkRouter composes a query string
   * from the key/value pairs not included in the <base>.
   *
   * The default atkRouter setup supports a typical path of "controller/action/id" 
   * where action and id are optional, with action defaulting to ÔindexÕ when 
   * not given. Here are some typical url statements and their corresponding URLs: 
   *
   * $this->url(array('controller' => 'posts', 'action' => 'recent') // => 'proto://host.com/posts/recent'
   * $this->url(array('controller' => 'posts', 'action' => 'index') // => 'proto://host.com/posts'
   * $this->url(array('controller' => 'posts', 'action' => 'show', 'id' => 10) // => 'proto://host.com/posts/show/10'
   * 
   * When generating a new URL, missing values may be filled in from the 
   * current requestÔs parameters. For example, $this->url(array('action' => 'some_action'))
   * will retain the current controller, as expected. This behavior extends to 
   * other parameters, including 'controller', 'id', and any other parameters 
   * that are placed into a routeÔs path. 
   * 
   * The URL Smarty plug-ins, such as {url}, have a limited form of memory: 
   * when generating a new URL, they can look for missing values in the 
   * current requestÔs parameters. atkRouter attempts to guess when a value 
   * should and should not be taken from the defaults. There are a few simple
   * rules on how this is performed:
   * 
   * If the controller name begins with a slash, no defaults are used: 
   * $this->url(array('controller' => '/home'))
   * 
   * If the controller changes, the action will default to index unless 
   * provided.
   * 
   * The final rule is applied while the URL is being generated and is best 
   * illustrated by an example. Let us consider the route given by 
   * $map->connect('people/:last/:first/:actionÕ, array('action' => 'bio', 'controller' => 'people'))
   * 
   * Suppose that the current URL is "people/doe/john/contacts". LetÔs consider
   * a few different cases of URLs which are generated from this page.
   * 
   * $this->url(array('action' => ÔbioÔ)) Ñ During the generation of this URL, 
   * default values will be used for the first and last components, and the 
   * action shall change. The generated URL will be, "people/doe/john/bio".
   *
   * $this->url(array('first' => 'jane')) - This generates the URL Ôpeople/doe/janeÕ Ñ 
   * note that this URL leaves out the assumed action of ÔbioÕ.
   * 
   * However, you might ask why the action from the current request, ÔcontactsÕ, 
   * isnÔt carried over into the new URL. The answer has to do with the order 
   * in which the parameters appear in the generated path. In a nutshell, 
   * since the value that appears in the slot for :first is not equal to 
   * default value for :first we stop using defaults. On itÔs own, this rule
   * can account for much of the typical URL behavior.
   * 
   * Although a convienence, defaults can occasionaly get in your way. In some
   * cases a default persists longer than desired. The default may be cleared
   * by adding 'name' => null to the parameters. The following line will 
   * redirect to PostControllerÔs default action, regardless of the page it is
   * displayed on:
   *
   * $this->url(array('controller' => 'posts', 'action' => null))
   * 
   * If you explicitly want to create an URL thatÔs almost the same as the 
   * current URL, you can do so using the :overwrite_params option. Say for 
   * your posts you have different views for showing and printing them. 
   * Then, in the show view, you get the URL for the print view like this:
   *
   * $this->url(array('overwrite_params' => array('action' => 'print')))
   * 
   * This takes the current URL as is and only exchanges the action. In 
   * contrast, $this->url(array('action' => 'print')) would have 
   * slashed-off the path components after the changed action.
   *
   * @param array $params URL parameters
   * @return string url
   */
  public function url($params=array())
  {
    return $this->m_url->rewrite($params);
  }

  /**
   * Redirect to the given url.
   *
   * @param string $url The URL.
   */
  protected function doRedirect($url)
  {
    $this->m_bridge->doRedirect($url);
  }

  /**
   * Redirect to the given url.
   *
   * @param array $vars request vars
   * @return string url
   */
  protected function redirect($vars=array(), $force=FALSE)
  {
    if ($this->isRoot() || $force)
    {
      $this->doRedirect($this->url($vars));
    }
    else
    {
      $content = self::dispatchRequest($vars);
      $this->render($content);
    }
  }

  protected function render($options=array())
  {
    if ($this->isRendered())
    {
      throw new atkFrontException(atkFrontException::DOUBLE_RENDER_ERROR, "Render has already been called.");
    }
    
    $result = $this->m_view->render($options);  
    $this->response->setBody($result);
    
    $this->setRendered(true);
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
  
  public function __call($method, $params)
  {
    $helpers = $this->getHelpers();
    foreach ($helpers as $helper)
    {
      if (method_exists($helper, $method))
      {
        return call_user_func_array(array($helper, $method), $params);
      }
    }
    
    throw new Exception('Invalid method');
  }
  
  /**
   * To string.
   */
  public function __toString()
  {
    return (!$this->isRoot() ? $this->getParent()->__toString().'/' : '').$this->getName();
  }  
}
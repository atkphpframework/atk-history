<?php
abstract class atkView
{
  private $m_controller;
  private $m_tags = null;
  private $m_blocks = null;
  
  public static function create($controller)
  {
    atkimport('atk.front.view.atksmartyview');
    return new atkSmartyView($controller);
  }
  
  protected function __construct($controller)
  {
    $this->m_controller = $controller;
  }

  protected function getController()
  {
    return $this->m_controller;
  }
  
  protected function getHelperTags()
  {
    if ($this->m_tags == null)
    {
      $this->m_tags = array();
      
      $helpers = $this->getController()->getHelpers();
      foreach ($helpers as $helper)
      {
        $tags = $helper->getTags();
        foreach ($tags as $name => $handler)
        {
          $this->m_tags[$name] = $handler;
        }
      }
    }
    
    return $this->m_tags;
  }
  
  protected function getHelperBlocks()
  {
    if ($this->m_blocks == null)
    {
      $this->m_blocks = array();
      
      $helpers = $this->getController()->getHelpers();
      foreach ($helpers as $helper)
      {
        $blocks = $helper->getBlocks();
        foreach ($blocks as $name => $handler)
        {
          $this->m_blocks[$name] = $handler;
        }
      }
    }
    
    return $this->m_blocks;    
  }
  
  /**
   * Does a template exist for the current controller action?
   * 
   * @return boolean template exists?
   */
  public function templateExists()
  {
    $template = $this->getController()->getName().'/'.$this->getController()->getAction().'.tpl';
    $filename = $this->getTemplatePath($template);
    return $filename != null;
  }
  
  
  public function render($options=array())
  {    
    $vars = is_array(@$options['vars']) ? $options['vars'] : array();
    $vars = array_merge($this->getController()->getVars(), $vars);
    
    $layout = @$options['layout'];
    
    if (isset($options['text']))
    {
      $layout = $layout !== null ? $layout : false;      
      $result = $this->renderText($options['text']);
    }
    else if ($file = @$options['file'])
    {
      $layout = $layout !== null ? $layout : false;      
      $result = $this->renderFile($file, $vars);
    }
    else if (isset($options['template']))
    {
      $layout = $layout !== null ? $layout : true;      
      $result = $this->renderTemplate($options['template'], $vars);
    }
    else if ($inline = @$options['inline'])
    {
      $layout = $layout !== null ? $layout : false;
      $result = $this->renderInline($inline, $vars);
    }
    else if (isset($options['action']))
    {
      $layout = $layout !== null ? $layout : true;
      $result = $this->renderAction($options['action'], $vars);
    }
    else if (isset($options['partial']))
    {
      $layout = $layout !== null ? $layout : false;
      $result = $this->renderPartial($options['partial'], $vars);
    }
    else if (isset($options['nothing']))
    {
      $layout = $layout !== null ? $layout : false;
      $result = $this->renderText(' ');
    }
    else if (isset($options['json']))
    {
      $layout = $layout !== null ? $layout : false;
      $result = $this->renderJSON($options['json']);
    }
    else 
    {
      $layout = $layout !== null ? $layout : true;      
      $result = $this->renderAction($this->getController()->getAction(), $vars);
    }
    
    if ($layout)
    {
      $result = $this->renderLayout($layout, $vars, $result);
    }
    
    return $result;
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
    $packagePath = $this->getController()->getPackagePath();
    
    if (file_exists(atkconfig('atkroot')."$directory/$template"))
    {
      return atkconfig('atkroot')."$directory/$template";
    }
    else if (file_exists($packagePath."../$directory/$template"))
    {
      return $packagePath."../$directory/$template";
    }
    else if (file_exists($packagePath."../skel/$directory/$template"))
    {
      return $packagePath."../skel/$directory/$template";
    }
    else
    {
      return null;
    }
  }

  /**
   * Returns the layout template path for this controller.
   */
  protected function getLayoutPath($layout=null)
  {
    if ($layout != null)
    {    
      return $this->getTemplatePath($layout.'.tpl', 'layouts');
    }
    
    $path = $this->getTemplatePath($this->getController()->getName().'.tpl', 'layouts');
    if ($path != null) 
    {
      return $path;
    }

    $path = $this->getTemplatePath('root.tpl', 'layouts');
    return $path;
  }  
  
  protected function renderText($text)
  {
    return $text;
  }
  
  protected function renderTemplate($template, &$vars)
  {
    $template = $template.'.tpl';
    $filename = $this->getTemplatePath($template);
    return $this->renderFile($filename, $vars);    
  }
  
  protected function renderAction($action, &$vars)
  {
    $template = $this->getController()->getName().'/'.$action.'.tpl';
    $filename = $this->getTemplatePath($template);
    return $this->renderFile($filename, $vars);
  }
  
  protected function renderPartial($partial, &$vars)
  {
    $parts = explode('/', $partial);
   
    if (count($parts) == 1)
    {
      $template = $this->getController()->getName().'/_'.$partial.'.tpl';
    }
    else 
    {
      $template = implode('/', array_slice($parts, 0, -1)).'/_'.end($parts).'.tpl';
    }
    
    $filename = $this->getTemplatePath($template);
    return $this->renderFile($filename, $vars);
  }
  
  protected function renderLayout($layout, $vars, $result)
  {
    $layout = is_string($layout) ? $layout : null;
    $filename = $this->getLayoutPath($layout);
    $vars['content_for_layout'] = $result;
    return $this->renderFile($filename, $vars);
  }
  
  protected function renderJSON($data)
  {
    atkimport('atk.utils.atkjson');
    return $this->renderText($data == null ? 'null' : atkJSON::encode($data));
  }
  
  protected abstract function renderFile($filename, &$vars);
  
  protected abstract function renderInline($template, &$vars);  
}
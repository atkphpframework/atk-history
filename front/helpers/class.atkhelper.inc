<?php
class atkHelper
{
  /**
   * View tags.
   *
   * @var array
   */
  private $m_tags = array();
  
  /**
   * View blocks.
   * 
   * @var array
   */
  private $m_blocks = array();
  
  /**
   * Controller methods.
   * 
   * @var array
   */
  private $m_methods = array();
  
  /**
   * Controller.
   * 
   * @var atkFrontController
   */
  private $m_controller;
  
  /**
   * Constructor.
   * 
   * @param atkFrontController $controller   controller instance
   * @param boolean            $autoRegister auto-register tags, blocks and other methods
   */
  public function __construct($controller, $autoRegister=true)
  {
    $this->m_controller = $controller;
    
    if ($autoRegister)
    {
      $this->autoRegister();
    }
  }
  
  /**
   * Get controller.
   * 
   * @return atkFrontController
   */
  protected function getController()
  {
    return $this->m_controller;
  }
  
  /**
   * Auto-register tags, blocks and other methods.
   */
  private function autoRegister()
  {
    $object = new ReflectionObject($this);
    $methods = $object->getMethods(ReflectionMethod::IS_PUBLIC);
    
    foreach ($methods as $method)
    {
      if (preg_match('/^(.+)Tag$/i', $method->getName(), $matches))
      {
        $this->registerTag($matches[1], $matches[0]);
      }
      else if (preg_match('/^(.+)Block$/i', $method->getName(), $matches))
      {
        $this->registerBlock($matches[1], $matches[0]);
      }
      else
      {
        $this->registerMethod($method->getName());
      }
    }
  }
  
  /**
   * Register view tag.
   *
   * @param string $name    tag name
   * @param string $handler method name (for this helper), callable or 
   *                        null (if null tag name will be used) 
   */
  protected function registerTag($name, $handler=null)
  {
    $this->register($name, $handler, $this->m_tags);
  }
  
  /**
   * Unregister view tag.
   * 
   * @param string $name tag name
   */
  protected function unregisterTag($name)
  {
    unset($this->m_tags[$name]);
  }
  
  /**
   * Register view block.
   *
   * @param string $name    block name
   * @param string $handler method name (for this helper), callable or 
   *                        null (if null tag name will be used) 
   */
  protected function registerBlock($name, $handler=null)
  {
    $this->register($name, $handler, $this->m_blocks);
  }
  
  /**
   * Unregister view block.
   * 
   * @param string $name block name
   */
  protected function unregisterBlock($name)
  {
    unset($this->m_blocks[$name]);
  }  
  
  /**
   * Register controller helper method.
   *
   * @param string $name    method name inside controller
   * @param string $handler method name (for this helper), callable or 
   *                        null (if null tag name will be used) 
   */
  protected function registerMethod($name, $handler=null)
  {
    $this->register($name, $handler, $this->m_methods);
  }
  
  /**
   * Unregister controller helper method.
   * 
   * @param string $name method name
   */
  protected function unregisterMethod($name)
  {
    unset($this->m_methods[$name]);
  }  
  
  /**
   * Register tag, block or method with the given name and
   * optional handler in the given array.
   * 
   * @param string $handler method name (for this helper), callable or 
   *                        null (if null tag name will be used) 
   */
  protected function register($name, $handler, &$list)
  {
    if ($handler == null)
    {
      $handler = array($this, $name);
    }
    else if (is_string($handler) && method_exists($this, $handler))
    {
      $handler = array($this, $handler);
    }
    
    $list[$name] = $handler;    
  }
  
  /**
   * Returns the view tags this helper supplies.
   * 
   * @return array view tags
   */
  public function getTags()
  {
    return $this->m_tags;
  }
  
  /**
   * Returns the view blocks this helper supplies.
   * 
   * @return array view blocks
   */
  public function getBlocks()
  {
    return $this->m_blocks;
  }
  
  /**
   * Returns the controller methods this helper supplies.
   * 
   * @return array controller methods
   */
  public function getMethods()
  {
    return $this->m_methods;
  }
}
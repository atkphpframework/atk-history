<?php
/**
 * Factory class for finding and creating controllers with a given name.
 * 
 * @author Peter C. Verhage
 * @version $Revision$
 */
class atkFrontControllerFactory
{
  /**
   * Controller index.
   *
   * @var array
   */
  private $m_index = null;
  
  /**
   * Controller index by package.
   * 
   * @var array
   */
  private $m_indexByPackage = null;
  
  /**
   * The only instance.
   *
   * @var atkFrontControllerTraverser
   */
  private static $s_instance;

  /**
   * Returns the only instance.
   *
   * @return atkFrontControllerTraverser instance
   */
  private static function getInstance()
  {
    if ($s_instance == null)
    {
      $s_instance = new self();
    }
    
    return $s_instance;
  }
  
  /**
   * Constructor can only be called internally.
   */
  private function __construct()
  {
  }
  
  /**
   * Lookup controller path for the controller with the given name.
   * 
   * By supplying a package the given package will be searched first or
   * exclusively if the $packageOnly parameter is set to true.
   * 
   * If the controller cannot be found null will be returned.
   *
   * @param string  $controller  controller name
   * @param string  $package     controller package (optional)
   * @param boolean $packageOnly only search the given package (if not null)
   * 
   * @return string controller ATK import path
   */
  public static function lookup($controller, $package=null, $packageOnly=false)
  {
    return self::getInstance()->lookupPath($controller, $package, $packageOnly);
  }
  
  /**
   * Create controller with the given name. If no controller can be found
   * null is returned.
   * 
   * @param string  $controller  controller name
   * @param string  $package     controller package (optional)
   * @param boolean $packageOnly only search the given package (if not null)
   * 
   * @return atkFrontController controller instance
   */
  public static function create($controller, $package=null, $packageOnly=false)
  {
    $path = self::lookup($controller, $package, $packageOnly);
    
    if ($path == null) 
    {
      throw new atkFrontException(atkFrontException::INVALID_CONTROLLER, "No controller found for '{$controller}'");
    }
    
    return atkFrontController::create($path);
  }
  
  /**
   * Lookup controller path.
   *
   * @param string  $controller  controller name
   * @param string  $package     package name
   * @param boolean $packageOnly only search the given package (if not null)
   * 
   * @return string controller (import) path
   */ 
  private function lookupPath($controller, $package, $packageOnly=false)
  {
    if ($this->m_index == null)
    {
      $this->indexAll();
    }
    
    if ($package != null)
    {
      $path = @$this->m_indexByPackage[$package][$controller];
    }
    
    if ($package == null || ($path == null && !$packageOnly))
    {
      $path = @$this->m_index[$controller];
    }
    
    return $path;
  }
  
  /**
   * Build controller index.
   */
  private function indexAll()
  {
    $this->m_index = array();
    $this->m_indexByPackage = array();
    
    $this->indexPackage('controllers');
    
    $modules = atkGetModules();
    foreach (array_keys($modules) as $module)
    {
      $this->indexPackage('module.'.$module.'.controllers');
    }
  }
  
  /**
   * Index the given package.
   * 
   * @param string $package package
   */
  private function indexPackage($package)
  {
    $path = getClassPath($package, false);
    
    $iter = new DirectoryIterator($path);
    foreach ($iter as $file)
    {  
      if (preg_match('/class.([a-z][a-z0-9_]*)controller.inc$/', $file, $matches))
      {
        $this->indexController($matches[1], $package);
      }
    }
  }
  
  /**
   * Index the given controller.
   * 
   * @param string $controller controller name
   * @param string $module     package name
   */
  private function indexController($controller, $package)
  {        
    $class = $package.'.'.$controller.'controller'; 
    
    // the first controller with the given name wins
    if (!isset($this->m_index[$controller]))
    { 
      $this->m_index[$controller] = $class;
    }
    
    $this->m_indexByPackage[$package][$controller] = $class;    
  }
}
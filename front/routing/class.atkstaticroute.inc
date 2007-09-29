<?php
atkimport('atk.front.routing.atkrouteinterface');

/**
 * Static route class.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkStaticRoute extends atkRoute
{
  protected $m_route;
  protected $m_defaults;
  protected $m_requirements;

  /**
   * Constructor.
   *
   * @param string $route
   * @param array  $defaults
   * @param array  $requirements
   */
  public function __construct($route, $defaults=array(), $requirements=array())
  {
    $this->m_route = $route;
    $this->m_defaults = array_merge(array('action' => 'index'), $defaults);
    $this->m_requirements = $requirements;  
  }
  
  /**
   * Does this route match the given path? If so the matching
   * values will be returned. If no match, false is returned.
   *
   * @param string $path request path
   * @param array  $env  request environment
   * 
   * @return array|null matching values or null
   */
  public function recognize($path, $env=array())
  {
    if ($path == $this->m_route && $this->validEnvironment($env, $this->m_requirements))
    {
      return $this->m_defaults;
    }
    else 
    {
      return false;
    }
  }  

  /**
   * Build URL for the given values.
   * 
   * @param $params url parameters
   * 
   * @return string url for the given values
   */
  public function generate($params, &$options)
  {
    foreach ($this->m_defaults as $name => $value)
    {
      if (!isset($params[$name]) || $params[$name] != $value)
      {
        return false;
      }
    }
    
    return true;
  }
}
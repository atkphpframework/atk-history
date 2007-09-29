<?php
atkimport('atk.front.route.atkroute');

/**
 * Storage container for the atkRouter routes
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkRouterMap
{
  /**
   * Routes.
   *
   * @var array
   */
  private $m_routes;
  
  /**
   * Named routes.
   * 
   * @var array
   */
  private $m_namedRoutes;
  
  /**
   * Constructor.
   * 
   * @param array reference to the route array
   */
  public function __construct(&$routes, &$namedRoutes)
  {
    $this->m_routes = &$routes;
    $this->m_namedRoutes = &$namedRoutes;
  }
     
  /**
   * Connect route.
   * 
   * @param atkRoute|string $route        route instance or route string compatible with atkRoute
   * @param array           $defaults     array containing the default values
   * @param array           $requirements array containing regular expression that check if the values match a certain format
   * @param string          $name         route name
   * 
   * @return atkRoute route instance
   */
  function connect($route, $defaults=array(), $requirements=array())
  {
    if (is_string($route))
    {
      $route = atkRoute::create($route, $defaults, $requirements);
    }
    
    $this->m_routes[] = $route;
    
    return $route;
  }
  
  /**
   * Connect a named route.
   */
  public function __call($method, $params)
  {
    $route = $params[0];
    $defaults = isset($params[1]) && is_array($params[1]) ? $params[1] : array();
    $requirements = isset($params[2]) && is_array($params[2]) ? $params[2] : array();    
    $name = $method;
    
    $route = $this->connect($route, $defaults, $requirements, $name);
    $this->m_namedRoutes[$name] = $route;
  }
}
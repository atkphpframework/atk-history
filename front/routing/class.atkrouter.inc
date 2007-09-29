<?php
atkimport('atk.front.routing.atkroutermap');

/**
 * Front controller router.
 */
class atkRouter
{
  /**
   * Router instance.
   * 
   * @var atkRouter
   */
  private static $s_instance = null;  
  
  /**
   * Router map, can be used to add routes.
   * 
   * @var atkRouterMap
   */
  public $map;
  
  /**
   * Routes.
   * 
   * @var array
   */
  private $m_routes = array();
  
  /**
   * Named routes.
   * 
   * @var array
   */
  private $m_namedRoutes = array();
  
  /**
   * Constructor.
   */
  private function __construct()
  {
    $this->map = new atkRouterMap($this->m_routes, $this->m_namedRoutes);
  }
  
  /**
   * Returns the atkRouter instance.
   * 
   * @return atkRouter
   */
  public function getInstance()
  {
    if (self::$s_instance == null)
    {
      self::$s_instance = new atkRouter();
    }
    
    return self::$s_instance;
  }
  
  /**
   * Clear routes.
   */
  public function clear()
  {
    $this->m_routes = array();
  }
  
  /**
   * (Re-)load routes.
   */
  public function load()
  {
    $this->reload();
  }
  
  /**
   * (Re-)load routes.
   */
  public function reload()
  {
    $this->clear();
    
    $file = atkconfig('atkroot').'routes.php';
    
    if (file_exists($file))
    {
      $map = $this->map;
      include($file);
    }
    else 
    {
      $this->map->connect(':controller/:action/:id');
    }
  }
  
  /**
   * Generate path for the given parameters using the given named
   * route. The options array may contain options for the path generation. 
   *
   * @param string $name    route name
   * @param array  $params  parameter list
   * @param array  $options options
   * 
   * @return string path
   */  
  protected function generateNamedRoute($name, $params, &$options)
  {      
    $route = @$this->m_namedRoutes[$options['use_route']];
    if ($route == null)
    {  
      throw new atkRoutingException("Named route not found.");     
    }
    
    $path = $route->generate($params, $options);
    if ($path == null)
    {
      throw new atkRoutingException("Named route not compatible with the given request parameters.");    
    }
    
    return $path;
  }
  
  /**
   * Generate path for the given parameters. The options array may contain
   * options for the path generation. 
   *
   * @param array $params  parameter list
   * @param array $options options
   * 
   * @return string path
   */  
  protected function generateRoute($params, &$options)
  {    
    foreach ($this->m_routes as $route)
    {
      $path = $route->generate($params, $options);
      if ($path != null) 
      {
        return $path;
      }
    }
    
    throw new atkRoutingException("No route found to match the given request parameters."); 
  }
  
  /**
   * Generate path for the given parameters. The given recall array is used
   * to fill any blanks in the given parameters. The options array may contain
   * options for the path generation. 
   *
   * @param array $params  parameter list
   * @param array $recall  current request parameter list
   * @param array $options options
   * 
   * @return string path
   */
  public function generate($params, $recall=array(), &$options=array())
  {
    // if a controller is explicitly provided, but no action, we 
    // fallback to the default action, index
    if (@$params['controller'] && !@$params['action'])
    {
      $params['action'] = 'index';
    }
    
    // if the controller name start with a / we don't use the default values
    if (@$params['controller']{0} == '/')
    {
      $params['controller'] = substr($params['controller'], 1);
      $recall = array();
    }
    
    // the final parameters
    $params = array_merge($recall, $params);
    if ($params['action'] == null)
    {
      $params['action'] = 'index';
    }
    
    if (@$options['use_route'])
    {
      return $this->generateNamedRoute($options['use_route'], $params, $options);
    }
    else 
    {
      return $this->generateRoute($params, $options);
    }
  }
  
  /**
   * Find a controller that fits the given request.
   *
   * @param atkRequest $request request object
   * 
   * @return atkFrontController controller instance
   */
  public function recognize($request)
  {
    $env = $this->extractRequestEnvironment($request);
    $params = $this->recognizePath($request->getPath(), $env);
    $request->setPathParameters($params);
    $controller = atkFrontControllerFactory::create($params['controller'], @$params['package'], true);
    return $controller;
  }
  
  /**
   * Find the first route that matches the given path taking
   * the given environment into account and return the path
   * parameters for the found route.
   * 
   * @param string $path request path
   * @param array  $env  request environment
   * 
   * @return array path parameters
   * 
   * @throws atkRoutingException if no route can be found
   */
  protected function recognizePath($path, $env=array())
  {
    $path = trim($path, '/');
    
    foreach ($this->m_routes as $route)
    {
      $result = $route->recognize($path, $env);
      
      if ($result !== false)
      {
        return $result;
      }
    }
    
    throw new atkFrontException(atkFrontException::NO_ROUTE_FOUND, "No route found to match '{$path}' with environment ".var_export($env, true));
  }
  
  /**
   * Extract the request environment from the request, by
   * default we extract the request method, host, port and
   * protocol.
   * 
   * @param atkRequest request request object
   * 
   * @return array request environment
   */
  protected function extractRequestEnvironment($request)
  {
    $env = array();
    $env['method'] = $request->getMethod();
    $env['host'] = $request->getHost();
    $env['port'] = $request->getPort();
    $env['protocol'] = $request->getProtocol();
    return $env;
  }
}
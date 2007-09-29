<?php
/**
 * Route interface.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
abstract class atkRoute
{
  /**
   * Create a static or dynamic route for the given string
   * using the given defaults and requirements.
   * 
   * @param string $route        route string
   * @param array  $defaults     defaults
   * @param array  $requirements requirements
   * 
   * @return atkRoute
   */
  public static function create($route, $defaults=array(), $requirements=array())
  {
    $isStatic = strpos($route, ':') === false && strpos($route, '*') === false;
    
    if ($isStatic)
    {
      atkimport('atk.front.routing.atkstaticroute');
      return new atkStaticRoute($route, $defaults, $requirements);
    }
    else 
    {
      atkimport('atk.front.routing.atkdynamicroute');
      return new atkDynamicRoute($route, $defaults, $requirements);
    }
  }
  
  /**
   * Does this route match the given path? If so the matching
   * values will be returned. If no match, false is returned.
   *
   * @param string path the path to match
   * @return array|false matching values or false
   */
  public abstract function recognize($path, $env=array());
  
  /**
   * Build URL for the given values.
   * 
   * @param $values url parameters
   * @return string url for the given values
   */
  public abstract function generate($params, &$options);
  
  /**
   * Validate environment based on the given requirements.
   * 
   * This method only validates environment variable requirements for 
   * environment variables that are available in the environment array. 
   * Else there is no way to check if it's a requirement for an environment
   * variable or a normal variable.
   * 
   * @param array $environment  environment
   * @param array $requirements requirements
   * 
   * @return boolean environment valid?
   */
  protected function validEnvironment($environment, $requirements)
  {
    foreach ($environment as $name => $value)
    {
      if (isset($requirements[$name]) &&
          !preg_match($requirements[$name], $value))
      {
        return false;
      }
    }    
    
    return true;
  }
}
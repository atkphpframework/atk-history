<?php
atkimport('atk.front.routing.atkrouteinterface');

/**
 * Dynamic route.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkDynamicRoute extends atkRoute
{
  const SEPERATORS = '/;,.';
  
  const DYNAMIC_SEGMENT   = 'dynamic';
  const REMAINDER_SEGMENT = 'remainder';
  const STATIC_SEGMENT    = 'static';
  
  protected $m_route;
  protected $m_defaults;
  protected $m_requirements;
  protected $m_segments;

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
    $this->m_segments = $this->parse();
  }
  
  /**
   * Parse route and return its segments.
   *
   * @return array segments
   */
  protected function parse()
  {
    $parts = preg_split('/['.preg_quote(self::SEPERATORS, '/').']/', $this->m_route);
    $segments = array();
    
    foreach ($parts as $part)
    {
      $segment = $this->parseSegment($part);
      $segments[] = $segment;
    }
    
    return $segments;
  }
  
  /**
   * Parse segment.
   *
   * @param string $part URL part
   * 
   * @return stdClass segment
   */
  protected function parseSegment($part)
  {
    $segment = new stdClass();
      
    switch ($part{0})
    {
      case ':':
        $segment->type = self::DYNAMIC_SEGMENT;
        $segment->name = substr($part, 1);
        break;
      case '*':
        $segment->type = self::REMAINDER_SEGMENT;
        $segment->name = substr($part, 1);
        break;
      default:
        $segment->type = self::STATIC_SEGMENT;
        $segment->value = $part;  
    }
  
    if (isset($segment->name))
    {
      if (isset($this->m_defaults[$segment->name]))
      {
        $segment->default = $this->m_defaults[$segment->name];
      }
      
      if (isset($this->m_requirements[$segment->name]))
      {
        $segment->requirement = $this->m_requirements[$segment->name];
      }
    }    
    
    return $segment;
  }
  
  /**
   * Does this route match the given path? If so the matching
   * values will be returned. If no match, false is returned.
   *
   * @param string $path request path
   * @param array  $env  request environment
   * 
   * @return array|false matching values or false
   */
  public function recognize($path, $env=array())
  {
    $path = urldecode($path); // TODO
    $parts = preg_split('/['.preg_quote(self::SEPERATORS, '/').']/', $path);
    $values = $this->m_defaults;

    try 
    {
      foreach ($this->m_segments as $i => $segment)
      {
        // check if a part exists for this segment or if a default value is set
        if (!isset($parts[$i]) && !isset($segment->default))
        {
          throw new Exception('Missing segment.');
        }

        // retrieve default value for segment
        if (isset($segment->default))
        {
          $value = $segment->default;
        }

        // retrieve the value for the segment        
        if (isset($parts[$i]))
        {
          if ($segment->type == self::REMAINDER_SEGMENT)
          {
            $value = implode(array_slice($parts, $i), '/');
          }
          else 
          {
            $value = $parts[$i];
          }
        }

        // check requirement for segment
        if ($segment->type != self::STATIC_SEGMENT && isset($segment->requirement))
        {
          if (!preg_match($segment->requirement, $value))
          {
            throw new Exception('Invalid segment value.');
          }
        }
        
        // check value for static segment
        else if ($segment->type == self::STATIC_SEGMENT)
        {
          if ($segment->value != $value)
          {
            throw new Exception('Invalid segment value.');
          }
        }

        // assign value
        if ($segment->type == self::DYNAMIC_SEGMENT || $segment->type == self::REMAINDER_SEGMENT)
        {
          $values[$segment->name] = $value;
        }
      }
      
      if (!$this->validEnvironment($env, $this->m_requirements))
      {
        throw new Exception('Environment requirement not met');
      }

      return $values;
    }
    catch (Exception $ex)
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
    try
    {
      // check requirements
      foreach ($this->m_requirements as $name => $requirement)
      {
        if (!isset($params[$name]))
        {
          throw new Exception('Value not set.');
        }
        else if (!preg_match($requirement, $params[$name]))
        {
          throw new Exception('Invalid value.');
        }
      }
      
      $parts = array();
      foreach ($this->m_segments as $segment)
      {
        if ($segment->type == self::STATIC_SEGMENT)
        {
          $parts[] = $segment->value;
        }
        else if ($segment->type == self::DYNAMIC_SEGMENT || $segment->type == self::REMAINDER_SEGMENT)
        {
          if (!isset($params[$segment->name]) && !isset($this->m_defaults[$segment->name]))
          {
            throw new Exception('Segment value not found.');
          }
          else if (isset($params[$segment->name]))
          {
            $parts[] = $params[$segment->name];              
            unset($params[$segment->name]);
          }
          else if (isset($this->m_defaults[$segment->name]))
          {
            $parts[] = $this->m_defaults[$segment->name];              
          }
        }
      }
      
      $addTrailingSlash = false;
      foreach (array_reverse($this->m_segments, true) as $i => $segment)
      {
        if (($segment->type == self::DYNAMIC_SEGMENT || $segment->type == self::REMAINDER_SEGMENT) && isset($segment->default) && $parts[$i] == $segment->default)
        {
          array_pop($parts);
          $addTrailingSlash = true;
        }
        else 
        {
          break;
        }
      }
      
      return implode('/', $parts).($addTrailingSlash ? '/' : '');
    }
    catch (Exception $ex)
    {
      return false;
    }
  }
}
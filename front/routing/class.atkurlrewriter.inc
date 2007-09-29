<?php

/**
 * URL rewriter.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkUrlRewriter
{  
  /**
   * Request object.
   *
   * @var atkRequest
   */
  private $m_request;
  
  /**
   * Constructor.
   *
   * @param atkRequest $request request object
   */
  public function __construct($request)
  {
    $this->m_request = $request;
  }
 
  /**
   * Rewrite.
   */
  public function rewrite($params=array())
  {
    $options = $this->extractOptions($params);
    $path = $this->getPath($params, $options);
    return $this->getUrl($path, $params, $options);
  }
  
  /**
   * Extract options from the parameters. The given parameters
   * array will be modified; all found options will be removed
   * and placed in a special options array that will be returned.
   * 
   * @param array $params parameters
   * 
   * @return array extracted options
   */
  private function extractOptions(&$params)
  {
    $options = array();
    
    foreach (array('anchor', 'only_path', 'params', 'overwrite_params', 'use_route') as $name => $value)
    {
      if (isset($params[$name]))
      {
        unset($params[$name]);
        $options[$name] = $value;
      }
    }
    
    return $options;
  }
  
  /**
   * Returns the URL for the given path and parameters.
   */
  private function getUrl($path, $params)
  {
    $url = '';
    
    if (!@$options['only_path'])
    {
      $url .= $this->getProtocol();
      $url .= $this->getHostWithPort($options);
    }
    
    $url .= $this->m_request->getBasePath().'/'; // TODO
    $url .= $path;
    
    if (@$options['anchor'])
    {
      $url .= '#'.urlencode($options['anchor']);
    }
    
    return $url;   
  }
  
  /**
   * Returns the path for the given parameters.
   * 
   * @param $params  parameters
   * @param $options options 
   * 
   * @return string path (with query string appended!)
   */
  private function getPath($params, &$options)
  {
    if (is_array(@$options['params']))
    {
      $params = array_merge($params, $options['params']);
    }
    
    if (is_array(@$options['overwrite_params']))
    {
      $params = array_merge($params, $this->m_request->getParameters());
      $params = array_merge($params, $options['overwrite_params']);
    }
    
    return atkRouter::getInstance()->generate($params, $this->m_request->getParameters(), $options);
  }
  
  /**
   * Returns the protocol based on the given options and the current request.
   *
   * @param array $options options
   * 
   * @return string protocol (http:// or https://)
   */
  private function getProtocol($options)
  {
    if (@$options['protocol'])
    {
      return (substr($options['protocol'], -3) == '://' ? '' : '://').$options['protocol'];
    }
    else 
    {
      return $this->m_request->getProtocol();
    }
  }
  
  /**
   * Is the current protocol https?
   *
   * @param array $options options
   * 
   * @return boolean is https?
   */
  private function isSSL($options)
  {
    return $this->getProtocol($options) == atkRequest::HTTPS;
  }
  
  /**
   * Returns the standard port for the current protocol.
   *
   * @param array $options options
   * 
   * @return int standard port for current protocol
   */
  private function getStandardPort($options)
  {
    return $this->isSSL($options) ? 443 : 80;
  }
  
  /**
   * Is the given port in the options the standard port for the 
   * current protocol?
   *
   * @param array $options options
   * 
   * @return boolean is standard port?
   */
  private function isStandardPort($options)
  {
    return $options['port'] == $this->getStandardPort($options); 
  }
  
  /**
   * Returns the host with (optional) port for the given options
   * taking the current request into account.
   *
   * @param array $options options
   * 
   * @return string host with (optional) port
   */
  private function getHostWithPort($options)
  {
    if (@$options['host'] && @$options['port'] && !$this->isStandardPort($options['port']))
    {
      return $options['host'].':'.$options['port'];
    }
    else if (@$options['host'])
    {
      return $options['host'];
    }
    else if (@$options['port'] && !$this->isStandardPort($options['port']))
    {
      return $this->m_request->getHost().':'.$options['port'];
    }
    else 
    {
      return $this->m_request->getHostWithPort();
    }
  }

  /**
   * Retrieve anchor from the given options (if applicable).
   *
   * @param array $options
   * 
   * @return string anchor
   */
  private function getAnchor($options)
  {
    return @$options['anchor'] ? '#'.urlencode($options['anchor']) : '';
  }
}
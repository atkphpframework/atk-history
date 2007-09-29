<?php
atkimport('atk.front.atksession');

class atkRequest implements ArrayAccess 
{
  const GET    = "GET";
  const POST   = "POST";
  const PUT    = "PUT";
  const DELETE = "DELETE";
  const HEAD   = "HEAD";
  
  const HTTP   = "http://";
  const HTTPS  = "https://";
  
  private $m_requestParameters;
  private $m_pathParameters;
  private $m_parameters;
  private $m_session;
  
  public function __construct($params=null)
  {
    if ($params == null)
    {
      $params = array_merge($_GET, $_POST);
    }
    
    $this->m_requestParameters = $params;
    $this->m_session = new atkSession();
  }
  
  public function getSession()
  {
    $this->m_session;
  }

  public function getPath()
  {
    list($path) = explode('?', $this->getRequestUri());

    if ($path == null)
    {
      $path = '';
    }

    $path = preg_replace('/^'.preg_quote($this->getBasePath(), '/').'/', '', $path);

    return $path;
  }
  
  public function getBasePath()
  {
    if (atkconfig('front_base') != null)
    {
      $base = atkconfig('front_base');
    }
    else if (isset($_SERVER['SCRIPT_NAME']))
    {
      $base = preg_replace('/\/index\.php$/', '', $_SERVER['SCRIPT_NAME']); 
    }
    else 
    {
      $base = '';
    }

    return $base;
  }
  
  public function getMethod()
  {
    return $_SERVER['REQUEST_METHOD'];
  }
  
  public function isGet()
  {
    return $this->getMethod() == self::GET;
  }
  
  public function isPost()
  {
    return $this->getMethod() == self::POST;
  }
  
  public function isPut()
  {
    return $this->getMethod() == self::PUT;  
  }  
  
  public function isDelete()
  {
    return $this->getMethod() == self::DELETE;
  }
  
  public function isHead()
  {
    return $this->getMethod() == self::HEAD;
  }
  
  public function getProtocol()
  {
    return $_SERVER['HTTPS'] && strtolower($_SERVER['HTTPS']) != 'off' ? self::HTTPS : self::HTTP;
  }
  
  public function isSSL()
  {
    return $this->getProtocol() == self::HTTPS;
  }
  
  public function getPort()
  {
    return (int)$_SERVER['SERVER_PORT'];
  }
  
  public function getStandardPort()
  {
    return $this->isSSL() ? 443 : 80;
  }
  
  public function getPortString()
  {
    return $this->getPort() != $this->getStandardPort() ? ':'.$this->getPort() : '';
  }
  
  public function getHost()
  {
    return atkHost();
  }
  
  public function getHostWithPort()
  {
    return $this->getHost().$this->getPortString();
  }
  
  public function getRequestParameters()
  {
    return $this->m_requestParameters;
  }
  
  public function getPathParameters()
  {
    return $this->m_pathParameters;
  }
  
  public function setPathParameters($params)
  {
    $this->m_pathParameters = $params;
    $this->m_parameters = null;
  }
  
  public function getParameters()
  {
    if ($this->m_parameters == null)
    {
      $this->m_parameters = array_merge($this->getRequestParameters(), $this->getPathParameters());  
    }
    
    return $this->m_parameters;
  }
  
  public function getRequestUri()
  {
    return $_SERVER['REQUEST_URI'];
  }
  
  public function getUrl()
  {
    return $this->getProtocol().$this->getHostWithPort().$this->getRequestUri();
  }
  
  public function getFormat()
  {
    $params = $this->getParameters();
    return $params['format'];
  }
  
  public function isXMLHttpRequest()
  {
    return $_SERVER['HTTP_X_REQUESTED_WITH'] != null && stripos($_SERVER['HTTP_X_REQUESTED_WITH'], 'XMLHttpRequest') !== false;
  }
  
  public function isXHR()
  {
    return $this->isXMLHttpRequest();
  }
  
  public function getRemoteAddr()
  {
    return $_SERVER['REMOTE_ADDR'];
  }
  
  public function offsetGet($key)
  {
    $params = $this->getParameters();
    return $params[$key];
  }
  
  public function offsetSet($key, $value)
  {
    // not supported
  }
  
  public function offsetUnset($key)
  {
    // not supported
  }
  
  public function offsetExists($key)
  {
    $params = $this->getParameters();
    return isset($params[$key]);
  }
}
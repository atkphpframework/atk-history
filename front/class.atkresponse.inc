<?php
/**
 * Reponse object.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkResponse
{
  /**
   * Request.
   * 
   * @var atkRequest
   */
  private $m_request;
  
  /**
   * Headers.
   *
   * @var array
   */
  private $m_headers;
  
  /**
   * Body
   *
   * @string body
   */
  private $m_body;
  
  /**
   * Constructor.
   */
  public function __construct($request)
  {
    $this->m_request = $request;
  }
  
  /**
   * Returns the mime type.
   *
   * @return string mime type
   */
  public function getContentType()
  {
    list($mimeType) = explode(';', $this->getHeader('Content-Type'));
    return empty($mimeType) ? null : $mimeType;
  }
  
  /**
   * Sets the mime type.
   *
   * @param string $mimeType mime type
   */
  public function setContentType($mimeType)
  {
    $encoding = $this->getCharset();
    $this->setHeader('Content-Type', $mimeType.($encoding != null ? "; charset={$encoding}" : '')); 
  }
  
  /**
   * Returns the encoding.
   *
   * @return string encoding
   */
  public function getCharset()
  {
    list(,$charset) = explode(';', $this->getHeader('Content-Type'));
    if (empty($charset)) 
      return null;
    list(,$encoding) = explode('=', $charset);
    return empty($encoding) ? null : trim($encoding);
  }
  
  /**
   * Sets the encoding.
   *
   * @param string $encoding encoding
   */
  public function setCharset($encoding)
  {
    $mimeType = $this->getContentType();
    $this->setHeader('Content-Type', ($mimeType != null ? $mimeType : 'text/html')."; charset={$encoding}");     
  }
  
  /**
   * Returns the value for the header with the given name.
   *
   * @param string $name header name
   * @return string header value
   */
  public function getHeader($name)
  {
    return @$this->m_headers[$name];
  }
  
  /**
   * Sets the header with the given name with the given value.
   *
   * @param string $name  header name
   * @param string $value header value
   */
  public function setHeader($name, $value)
  {
    $this->m_headers[$name] = $value;
  }
  
  /**
   * Sets the response body.
   *
   * @param string $body response body
   */
  public function setBody($body)
  {
    $this->m_body = $body;
  }
  
  /**
   * Returns the response body.
   *
   * @return string response body
   */
  public function getBody()
  {
    return $this->m_body;
  }
  
  /**
   * Redirect to the given URL.
   * 
   * @param string  $url         redirect URL
   * @param boolean $permanently redirect permanently?
   */
  public function redirect($url, $permanently=false)
  {
    $this->setHeader('Status', $permanently ? '301 Moved Permanently' : '302 Found');
    $this->setHeader('Location', $url);
    $this->setBody("<html><body>You are being <a href=\"{$url}\">redirected</a>.</body></html>");
  }
  
  /**
   * Explictly sets the content length based on
   * the current response body.
   */
  private function setContentLength()
  {
    $this->setHeader('Content-Length', strlen($this->m_body));
  }
  
  /**
   * Send response.
   */
  public function output()
  {
    $this->setContentLength();
    
    foreach ($this->m_headers as $name => $value)
    {
      header("{$name}: {$value}", true);
    }
    
    while (ob_end_clean());
    
    if (!$this->m_request->isHead())
    {
      echo $this->m_body;
    }
  }
}
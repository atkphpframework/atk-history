<?php
atkimport('atk.front.atkfrontcontrollerbridge');

/**
 * Session container.
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkSession implements ArrayAccess
{
  /**
   * Session data container.
   *
   * @var array
   */
  private $m_data;
  
  /**
   * Constructor.
   */
  public function __construct()
  {
    $this->m_data = &atkFrontControllerBridge::getSession();
  }
  
  /**
   * Returns the session variable with the given key.
   *
   * @param string $key session variable key
   * 
   * @return mixed session variable value
   */
  public function offsetGet($key)
  {
    return $this->m_data[$key];
  }
  
  /**
   * Set session variable value.
   *
   * @param string $key   session variable key
   * @param string $value session variable value
   */
  public function offsetSet($key, $value)
  {
    $this->m_data[$key] = $value;
  }
  
  /**
   * Unset the given session variable.
   *
   * @param string $key session variable key
   */
  public function offsetUnset($key)
  {
    unset($this->m_data[$key]);
  }
  
  /**
   * Checks if the given session variable exists.
   *
   * @param string $key session variable key
   * 
   * @return boolean session variable exists
   */
  public function offsetExists($key)
  {
    return isset($this->m_data[$key]);
  }
}
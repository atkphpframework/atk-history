<?php

/**
 * ATK front-end exception.
 * 
 * @author  Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkFrontException extends Exception 
{
  const INVALID_NAMED_ROUTE = 3;
  const INVALID_CONTROLLER  = 4;
  const UNKNOWN_ACTION      = 5;
  const RENDER_ERROR        = 6;
  const NO_ROUTE_FOUND      = 7;
  
  private $m_type;
  
  /**
   * Constructor.
   *
   * @param int    $type    exception type
   * @param string $message message
   */
  public function __construct($type, $message)
  {
    parent::__construct($message);
    $this->m_type = $type;
  }
  
  /**
   * Returns the exception type.
   * 
   * See the class constants.
   *
   * @return int exception type
   */
  public function getType()
  {
    return $this->m_type;
  }
  
  public function isInternal()
  {
    return !$this->isNotFound();
  }
  
  public function isNotFound()
  {
    return in_array($this->m_type, array(self::INVALID_NAMED_ROUTE, self::INVALID_CONTROLLER, self::UNKNOWN_ACTION));
  }
}
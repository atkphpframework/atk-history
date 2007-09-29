<?php
class atkSmartyViewWrapper
{
  private $m_handler;
  
  public function __construct($handler)
  {
    $this->m_handler = $handler;
  }
  
  public function invokeBlock($params, $content, $smarty, &$repeat)
  {
    return call_user_func_array($this->m_handler, array($params, $content, &$repeat));
  }
  
  public function invokeTag($params, $smarty)
  {
    return call_user_func_array($this->m_handler, array($params));
  }  
}
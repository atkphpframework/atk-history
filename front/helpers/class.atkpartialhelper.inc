<?php
class atkPartialHelper extends atkHelper
{
  /**
   * Render partial template.
   *
   * @param array  $params parameters
   * @param Smarty $smarty smarty instance
   * 
   * @return string rendered partial
   */
  public function partialTag($params)
  {
    $partial = $params[0];
    unset($params[0]);
    $options = array('partial' => $partial, 'locals' => $params, 'return' => true);
    return $this->getController()->render($options);
  }  
}
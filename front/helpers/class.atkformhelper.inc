<?php
class atkFormHelper extends atkHelper
{
  /**
   * Smarty function for converting the controller, action etc.
   * to hidden form vars.
   *
   * @param array $params parameters
   * @param Smarty $smarty smarty reference
   * @return string hidden form vars html
   */
  public function formBlock($params, $content, &$repeat)
  {
    if ($repeat) return null;
    
    $method = 'get';

    if (isset($params['method']))
    {
      $method = $params['method'];
      unset($params['method']);
    }
    
    $action = $this->getController()->url($params);
    
    $allowedAttribs = array('class', 'style', 'onsubmit');
    $attribsString = atkFrontTools::attributesString($params, $allowedAttribs);
    $attribsString = strlen($attribsString) > 0 ? ' '.$attribsString : '';
    
    $result = '
      <form method="'.$method.'" action="'.$action.'"'.$attribsString.'>
        '.$content.'
      </form>
    ';
    
    return $result;
  }
}
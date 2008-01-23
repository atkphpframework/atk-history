<?php
class atkFrontTools
{
  public static function camelize($string)
  {
    return str_replace(' ', '', ucwords(str_replace('_', ' ', $string)));
  }
  
  public static function underscore($string)
  {
    return strtolower(preg_replace('/([A-Z]+)/', '_$1', $string));
  }
  
  public static function attributesString($params, $allowed=null)
  {
  	$attribs = array();

  	if ($allowed != null)
  	{
  		$params = array_intersect_key($params, array_flip($allowed));
  	}
  	
  	foreach ($params as $param => $value)
  	{
  		$attribs[] = $param.'="'.htmlentities($value).'"';
  	}
  	
  	return implode(' ', $attribs);
  }
}
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
}
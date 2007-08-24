<?php

  if (atkconfig('autoload_classes', true))
  {
    atkimport('atk.utils.atkclassloader');
    function __autoload($classname)
    {
      // We exempt Smarty from autoloading because somehow it causes
      // a loop somewhere.
      if (!in_array($classname,array('Smarty','Services_JSON','pear','PEAR_Error')))
      {
        atkdebug("Starting autoload of class name: ".$classname);
        $classpath = atkClassLoader::findClass($classname);
        if ($classpath) $ret = atkimport($classpath);
        else if (strtolower(substr($classname,0,3))!=="atk")  atkerror("Class: $classname not found!");
        atkdebug("Ending autoload of class name: ".$classname);
      }
    }
  }

?>
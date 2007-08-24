<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage utils
   *
   * @copyright (c)2000-2007 Ibuildings BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * Utility for importing and loading definitions.
   * A definition is a PHP entity that (generally) resides in
   * its own file, like a class or an interface.
   *
   * @author Marc Veldman <marc@ibuildings.nl>
   *
   * @package atk
   * @subpackage utils
   *
   */
  class atkDefinitionLoader
  {
    /**
     * Includes definition files.
     *
     * @param string $fullname Name of class in atkformat e.g. (map1.map2.classfile)
     * @param bool   $failsafe      Determines wether a definition is included
     *                                or made required. Defaults to true.
     * @param bool   $path          Wether or not it is NOT an ATK classname
     *                                 ("map.class"), if true it will interpret classname
     *                                 as: "map/class.classname.inc", default false.
     * @return bool wether the file we want to import was actually imported or not
     */
    function loadDefinition($fullname, $failsafe=true, $path="", $type=DEFINITION_CLASS)
    {
      if (!$path) $filename = self::getDefinitionPath($fullname, true, $type);
      else        $filename = $fullclassname;

      if (file_exists($filename))
      {
        if ($failsafe) require_once($filename);
        else           include_once($filename);
        return true;
      }
      else return false;
    }
    
    /**
     * Get the path for a definition file, based on the atkformat.
     * e.g. (map1.map2.classfile) or map1.map2.interfacefile
     * 
     * @param String $fullname Name of definition in atkformat
     * @param boolean $useprefix Use the prefix (class.classname.inc or interface.interfacename.inc)
     * @param String $definitiontype Type of the definition. Now only class and interface are supported
     * @return String path of the definition file.
     */
    function getDefinitionPath($fullname, $useprefix=true, $definitiontype=DEFINITION_CLASS)
    {
      $elems = explode(".", strtolower($fullname));
      if($elems["0"] == "module")
      {
        array_shift($elems);
        $prefix = moduleDir(array_shift($elems));
      }
      else
      $prefix = atkconfig("atkroot");

      $last = &$elems[count($elems)-1];
      if ($useprefix) {$last = $definitiontype.".".$last.".inc";}

      $filename = $prefix.implode("/", $elems);

      return $filename;
    }
  }
?>
<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage utils
   * @access private
   *
   * @copyright (c)2007 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * Include the class we are about to test.
   */
  atkimport("atk.utils.atkdefinitionloader");  

  /**
   * Testcases for class atkdefinitionloader.
   *
   * @author Marc Veldman <marc@ibuildings.nl>
   * @package atk
   * @subpackage utils
   * @access private
   */
  class test_atkdefinitionloader extends atkTestCase 
  {        
    /**
     * Setup the testcase.
     */
    function setup()
    {          
    }
    
    function test_getDefinitionPath()
    {
      //Class
      $atkpath = 'map1.map2.testclass';
      $expected_path = atkconfig("atkroot").'map1/map2/class.testclass.inc';
      $this->assertEqual($expected_path, atkDefinitionLoader::getDefinitionPath($atkpath, true, DEFINITION_CLASS));
      //Interface
      $atkpath = 'map1.map2.testinterface';
      $expected_path = $expected_path = atkconfig("atkroot").'map1/map2/interface.testinterface.inc';
      $this->assertEqual($expected_path, atkDefinitionLoader::getDefinitionPath($atkpath, true, DEFINITION_INTERFACE));
      //No Prefix
      $atkpath = 'map1.map2.testinterface';
      $expected_path = $expected_path = atkconfig("atkroot").'map1/map2/testinterface';
      $this->assertEqual($expected_path, atkDefinitionLoader::getDefinitionPath($atkpath, false, DEFINITION_INTERFACE));
      //Module is special
      $class = 'testclass';
      $module = array_shift(array_keys(atkGetModules()));
      $atkpath = "module.{$module}.{$class}";
      $expected_path = moduleDir($module)."class.{$class}.inc";
      $this->assertEqual($expected_path, atkDefinitionLoader::getDefinitionPath($atkpath, true, DEFINITION_CLASS));
    }
  }
?>
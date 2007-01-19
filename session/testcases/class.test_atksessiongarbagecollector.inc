<?php
/**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage session
   * @access private
   *
   * @copyright (c)2006 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */


/**
   * Testcases for the atkSessionGarbageCollector
   *
   * @author Marc Veldman <marc@ibuildings.nl>
   * @package atk
   * @subpackage session
   * @access private
   */

atkimport("atk.session.atksessionvariable");

class test_atkSessionGarbageCollector extends atkTestCase
{
  var $config_identifier;
  var $session_vars;
  
  function setUp()
  {
    $this->config_identifier = atkconfig("identifier");
    $this->session_vars = new atkSessionGarbageCollector();
    parent::setUp();
  }
  
  function tearDown()
  {
    parent::tearDown();
    unset($this->session_vars);
  }

  function test_setVar()
  {
    //Test numeric.
    $test_int = rand(0,65535);
    $this->session_vars->setVar('setvar_int_test',$test_int);
    $this->assertIdentical($_SESSION[$this->config_identifier]['setvar_int_test'], $test_int, "Integer not correctly set");

    //Test string
    $test_string = 'TEST_STRING'.date('YmdHis');
    $this->session_vars->setVar('setvar_string_test',$test_string);
    $this->assertIdentical($_SESSION[$this->config_identifier]['setvar_string_test'], $test_string, "String not correctly set.");
    
    //Test array.
    $test_array = array(
      rand(0,65535),
      'Test Value'
    );
    $this->session_vars->setVar('setvar_string_test',$test_array);
    $this->assertIdentical($_SESSION[$this->config_identifier]['setvar_string_test'], $test_array, "Array not correctly set.");
  }

  function test_unsetVar()
  {
    $test_int_val = rand(0,65535);
    $this->session_vars->setVar('setvar_int_test',$test_int_val);
    $this->session_vars->unsetVar('setvar_int_test');
    $this->assertFalse(isset($_SESSION[$this->config_identifier]['setvar_int_test']),"Variable not unset");
  }
  
  function test_setTimeOut()
  {
    $test_int_val = rand(0,65535);
    $test_var_name = 'timeout_test';
    $this->session_vars->setTimeOut($test_var_name,$test_int_val);
    $this->assertIdentical($this->session_vars->getTimeOut($test_var_name), $test_int_val, "Timeout not set");
  }

  function test_unsetTimeOut()
  {
    $test_int_val = rand(0,65535);
    $test_var_name = 'timeout_test';
    $this->session_vars->setTimeOut($test_var_name,$test_int_val);
    $this->session_vars->unsetTimeOut($test_var_name);
    $this->assertFalse($this->session_vars->getTimeOut($test_var_name), "Timeout not unset");
  }
  
  function test_expireTimers()
  {
    //Timeout a simple value.
    $test_int_val = rand(0,65535);
    //First, test
    $this->session_vars->setTimeOut('setvar_int_test',5);
    $this->session_vars->setVar('setvar_int_test',$test_int_val);
    //Do a first cleanup.
    $this->session_vars->expireTimers();
    $this->assertTrue(isset($_SESSION[$this->config_identifier]['setvar_int_test']),"Integer should not have timed out yet");
    //Sleep for longer than the timeout.
    sleep(10);
    $this->session_vars->expireTimers();
    $this->assertFalse(isset($_SESSION[$this->config_identifier]['setvar_int_test']),"Integer should have timed out by now.");

    //Timeout a child based on a parent timeout.
    $test_int_val = rand(0,65535);
    $this->session_vars->setTimeOut('setvar_test_parent','*',5);
    $this->session_vars->setVar('setvar_test_parent','setvar_test_child',$test_int_val);
    //Do a first cleanup.
    $this->session_vars->expireTimers();
    $this->assertTrue(isset($_SESSION[$this->config_identifier]['setvar_test_parent']['setvar_test_child']),"Child should not have timed out yet");
    //Sleep for longer than the timeout.
    sleep(10);
    $this->session_vars->expireTimers();
    $this->assertFalse(isset($_SESSION[$this->config_identifier]['setvar_test_parent']['setvar_test_child']),"Child should have timed out by now.");
    
  }
}
?>
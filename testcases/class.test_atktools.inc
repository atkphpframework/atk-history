<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @access private
   *
   * @copyright (c)2007 Ibuildings
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * Testcases for class atkTools.
   *
   * @author Yury Golovnay <yury@achievo.org>
   * @package atk
   * @access private
   */

  atkimport("atk.atktools");

  class test_atktools extends atkTestCase
  {
    function test_atkArrayDiff()
    {
      $this->assertFalse(atkArrayDiff(array("a"=>null),array("a"=>null)));
      $this->assertFalse(atkArrayDiff(array("a"=>array(1)),array("a"=>array(1))));
      $this->assertEqual(atkArrayDiff(array("a"=>null),array("a"=>1)),array("a"=>null));
      $this->assertEqual(atkArrayDiff(array("a"=>1),array("a"=>null)),array("a"=>1));
    }

  }
?>
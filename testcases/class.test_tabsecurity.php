<?

  require_once(atkconfig("atkroot")."atk/test/simpletest/unit_tester.php"); 

  class mockSecurityManager
  {
    var $m_result = NULL;
    
    function setResult($result)
    {
      $this->m_result = $result;
    }
    
    function allowed($node, $privilege)
    {
      return $this->m_result;
    }
  }
  
  /**
   * Tests the tabs security
   *
   * ATK has now seperate security settings for different tabs within 
   * a node. This testcase tests the functionality.
   *
   * @author harrie <harrie@ibuildings.nl>
   */
  class test_tabsecurity extends UnitTestCase
  {
    function test_tabAllowed()
    { 
      global $g_securityManager, $g_nodes;

      // fake g_nodes
      // (advanced is a required tab)
      $g_nodes = array("unittest"=>array("testnode"=>array("tab_advanced")));
      
      $tabs = array("default", "advanced");
      
      $g_securityManager = &new mockSecurityManager();
      $g_securityManager->setResult(false);
      
      $myNode = new atkNode("testnode");
      $myNode->m_module="unittest";
      $myNode->checkTabRights($tabs);
      
      $this->assertEqual($tabs,array("default"),"Checking tabrights method");
    }
    
    function test_tabAllowed_backward_comp()
    { 
      global $g_securityManager, $g_nodes;

      // fake g_nodes
      // (advanced is a required tab)
      $g_nodes = array("unittest"=>array("testnode"=>array()));
      
      $tabs = array("default", "advanced");
      
      $g_securityManager = &new mockSecurityManager();
      $g_securityManager->setResult(false);
      
      $myNode = new atkNode("testnode");
      $myNode->m_module="unittest";
      $myNode->checkTabRights($tabs);
      
      $this->assertEqual($tabs,array("default","advanced"),"Checking tabrights method (backward compatibility)");
    }
  }

?>
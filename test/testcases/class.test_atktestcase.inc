<?php


  class test_atktestcase extends atkTestCase 
  {    
    function test_mockswap()
    {                 
      $db1 = &atkGetDb();      
      $this->assertNotEqual($db1->m_type, "mock", 'database is not a mock');
          
      $this->setMockDb();
      
      $db2 = &atkGetDb();                
        
      $this->assertEqual($db2->m_type, "mock", 'database is now a mock');
      
      $this->restoreDb();
      
      $db3 = &atkGetDb();                              
            
      $this->assertNotEqual($db3->m_type, "mock", 'database is now not a mock - '.$db3->m_type);
      
      $this->assertReference($db1, $db3, "make sure db is not copied");      
      
    }
        
  }

?>
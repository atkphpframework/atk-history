<?php
atkimport('atk.front.routing.atkstaticroute');

/**
 * Tests for static routes.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class Test_atkStaticRoute extends atkTestCase 
{
  /**
   * Test path recognition.
   */
  public function test_path_recognition()
  {
    $route = new atkStaticRoute('part1');    
    
    // invalid paths
    $this->assertFalse($route->recognize('part1/part2'));
    $this->assertFalse($route->recognize('part2'));
    
    // starting/ending slashes will be removed by atkRouter, so these should fail too        
    $this->assertFalse($route->recognize('part1/'));
    $this->assertFalse($route->recognize('/part1'));

    // valid path
    $params = $route->recognize('part1');
    $this->assertNotNull($params);
    $this->assertTrue(is_array($params));
    $this->assertEqual(0, count($params));   
    
    $route = new atkStaticRoute('part1/part2/part3', array('dummy' => 'dummy'));
    
    // invalid paths
    $this->assertFalse($route->recognize('part1'));
    $this->assertFalse($route->recognize('part1/part3/part2'));
    $this->assertFalse($route->recognize('part1/part2/part3/part4'));
    
    // starting/ending slashes will be removed by atkRouter, so these should fail too   
    $this->assertFalse($route->recognize('/part1/part2/part3')); 
    $this->assertFalse($route->recognize('part1/part2/part3/'));     
    
    // valid path
    $params = $route->recognize('part1/part2/part3');
    $this->assertNotNull($params);
    $this->assertTrue(is_array($params));
    $this->assertEqual(1, count($params));
    $this->assertTrue($params['dummy'] == 'dummy');
  }
  
  /**
   * Test environment requirements.
   */
  public function test_env_requirements()
  {
    // "simple" requirements
    $route = new atkStaticRoute('part1/part2/part3', array(), array('host' => '/^localhost$/'));
    $this->assertFalse($route->recognize('part1/part2/part3'));
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'www.achievo.org')));
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'WWW.ACHIEVO.ORG')));    
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'localhost')));
    
    $route = new atkStaticRoute('part1/part2/part3', array(), array('host' => '/^localhost|www\.achievo\.org$/i'));
    $this->assertFalse($route->recognize('part1/part2/part3'));
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'localhost.localdomain')));    
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'www.achievo.org')));
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'WWW.ACHIEVO.ORG')));    
    $this->assertTrue(is_array($route->recognize('part1/part2/part3', array('host' => 'localhost'))));
    
    // more "complex" requirements
    $route = new atkStaticRoute('part1/part2/part3', array(), array('host' => '/^.*\.achievo.org|achievo.org$/'));
    $this->assertFalse($route->recognize('part1/part2/part3'));
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'localhost')));    
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'invalidachievo.org')));        
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'achievo.org')));
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'www.achievo.org')));    
    
    $route = new atkStaticRoute('part1/part2/part3', array(), array('host' => '/^.*\.achievo.org$/', 'protocol' => '!https://!'));
    $this->assertFalse($route->recognize('part1/part2/part3'));
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'localhost')));    
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'invalidachievo.org')));        
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'achievo.org')));
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'www.achievo.org')));    
    $this->assertFalse($route->recognize('part1/part2/part3', array('host' => 'www.achievo.org', 'protocol' => 'http://')));        
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'achievo.org', 'protocol' => 'https://')));
    $this->assertNotNull($route->recognize('part1/part2/part3', array('host' => 'wiki.achievo.org', 'protocol' => 'https://')));   
  }  
  
  /**
   * Test path generation.
   */
  public function test_path_generation()
  {
    $options = array();
    $route = new atkStaticRoute('part1/part2/part3', array('a' => 'a'));
    $this->assertFalse($route->generate(array(), $options));
    $this->assertFalse($route->generate(array('a' => 'b'), $options));
    $this->assertFalse($route->generate(array('b' => 'a'), $options));    
    $this->assertEqual('part1/part2/part3', $route->generate(array('a' => 'a'), $options));        
    $this->assertEqual('part1/part2/part3', $route->generate(array('a' => 'a', 'y' => 'y'), $options));            
  }
}
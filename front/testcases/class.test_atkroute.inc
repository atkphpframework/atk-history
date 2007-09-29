<?php
/**
 * Test route factory.
 * 
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class Test_atkRoute extends atkTestCase
{
  /**
   * Test if we get an instance of the right route subclass based on the 
   * given path. Also test if the defaults and requirements are passed 
   * on properly.
   */
  public function test_route_creation()
  {
    $route = atkRoute::create('part1/part2/part3', array('dummy' => 'dummy'), array('host' => '/^localhost$/'));
    $this->assertTrue($route instanceof atkStaticRoute);
    $this->assertFalse($route->recognize('part1/part2/part3'));            
    $this->assertEqual($route->recognize('part1/part2/part3', array('host' => 'localhost')), array('dummy' => 'dummy'));        
    
    $route = atkRoute::create('part1/part2/:part3');
    $this->assertTrue($route instanceof atkDynamicRoute);    
    
    $route = atkRoute::create('part1/part2/*part3', array('dummy' => 'dummy'));
    $this->assertTrue($route instanceof atkDynamicRoute);    
    $this->assertEqual($route->recognize('part1/part2/blub'), array('dummy' => 'dummy', 'part3' => 'blub'));
  }
}
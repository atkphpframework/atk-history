<?php
atkimport("atk.front.route.atkrouter");

/**
 * Tests the atkRouter class.
 */
class test_atkrouter extends atkTestCase
{
  private $m_oldMap;
  
  /**
   * Setup routes.
   */
  public function setUp()
  {
    parent::setUp();
    $router = atkRouter::getInstance();
    $this->m_oldMap = $router->map;
    $router->map = new atkRouterMap();    
    $router->map->connect('static/route/example', array('module' => 'static', 'controller' => 'static'));    
    $router->map->connect('controllers/:module/:controller/:action', array('action' => 'index'));
    $router->map->connect('*url', array('module' => 'content', 'controller' => 'sphpage', 'action' => 'page'));    
  }
  
  /**
   * Reset routes.
   */
  public function tearDown()
  {
    parent::tearDown();

    $router = atkRouter::getInstance();
    $router->map = $this->m_oldMap;
  }
  
  /**
   * Test router.
   */
  public function test_router()
  {
    $router = atkRouter::getInstance();
    $this->assertEqual(3, count($router->map->routes));
  }
  
  /**
   * Test static routes.
   */
  public function test_static()
  {
    $request = array('key' => 'value');    
    $values = atkRouter::getInstance()->match('static/route/example', $request);                
    $this->assertTrue(is_array($values));
    $this->assertEqual('static', $values['module']);
    $this->assertEqual('static', $values['controller']);
    $this->assertEqual('index', $values['action']);    
    $this->assertEqual($request['key'], $values['key']);
    
    $path = atkRouter::getInstance()->url($values);
    $this->assertEqual('static/route/example', $path);
  }
  
  /**
   * Test dynamic routes.
   */
  public function test_dynamic()
  {
    $values = atkRouter::getInstance()->match('controllers/mymodule/mycontroller/myaction', array());            
    $this->assertTrue(is_array($values));
    $this->assertEqual('mymodule', $values['module']);
    $this->assertEqual('mycontroller', $values['controller']);    
    $this->assertEqual('myaction', $values['action']);
    
    $path = atkRouter::getInstance()->url($values);   
    $this->assertEqual('controllers/mymodule/mycontroller/myaction', $path);
    
    $values = atkRouter::getInstance()->match('controllers/mymodule/mycontroller', array());                
    $this->assertTrue(is_array($values));
    $this->assertEqual('mymodule', $values['module']);
    $this->assertEqual('mycontroller', $values['controller']);    
    $this->assertEqual('index', $values['action']);    

    $path = atkRouter::getInstance()->url($values);   
    $this->assertEqual('controllers/mymodule/mycontroller', $path);
    
    
    $values = atkRouter::getInstance()->match('controllers/mymodule/mycontroller/index', array());                
    $this->assertTrue(is_array($values));
    $this->assertEqual('mymodule', $values['module']);
    $this->assertEqual('mycontroller', $values['controller']);    
    $this->assertEqual('index', $values['action']);    

    $path = atkRouter::getInstance()->url($values);   
    $this->assertEqual('controllers/mymodule/mycontroller', $path);    
  }
  
  /**
   * Test remainer.
   */
  public function test_remainder()
  {
    $values = atkRouter::getInstance()->match('news/index', array('id' => 20));    
    $this->assertTrue(is_array($values));
    $this->assertEqual('content', $values['module']);    
    $this->assertEqual('sphpage', $values['controller']);        
    $this->assertEqual('page', $values['action']);  
    $this->assertEqual('news/index', $values['url']);
    $this->assertEqual(20, $values['id']);    
    
    $path = atkRouter::getInstance()->url($values);
    $this->assertTrue('news/index', $path);
    
    $values = atkRouter::getInstance()->match('news/index', array('url' => 'news/override'));    
    $this->assertTrue(is_array($values));
    $this->assertEqual('content', $values['module']);    
    $this->assertEqual('sphpage', $values['controller']);        
    $this->assertEqual('page', $values['action']);  
    $this->assertEqual('news/override', $values['url']);
    
    $path = atkRouter::getInstance()->url($values);
    $this->assertTrue('news/override', $path);    
  }
}
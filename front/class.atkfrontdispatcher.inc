<?php
atkimport('atk.front.atkfrontcontroller');
atkimport('atk.front.atkfrontcontrollerfactory');
atkimport('atk.front.atkrequest');
atkimport('atk.front.atkresponse');
atkimport('atk.front.routing.atkrouter');

/**
 * ATK front dispatcher. 
 * 
 * Convenience class for dispatching requests to the atkFrontController.
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkFrontDispatcher
{
  /**
   * Dispatch request.
   * 
   * @param array $params request parameters (optional)
   */
  public static function dispatch($params=null)
  {
    try 
    {
      $request = new atkRequest($params);
      $response = new atkResponse($request);
      
      $router = atkRouter::getInstance();
      $router->load();
      
      $controller = $router->recognize($request);
      $controller->process($request, $response)->output();
    }
    catch (Exception $exception)
    {
      $controller = null;
      
      try 
      {
        $controller = atkFrontControllerFactory::create('application');
      }
      catch (Exception $unused) 
      {}
      
      if ($controller == null)
      {
        $controller = atkFrontController::create('atk.front.atkapplicationcontroller');
      }
      
      $controller->processWithException($request, $response, $exception)->output();    
    }
  }
}
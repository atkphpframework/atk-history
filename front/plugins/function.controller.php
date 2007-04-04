<?php
/**
 * Controller plug-in.
 *
 * @param array $params
 * @param Smarty $smarty
 * @return string result
 */
function smarty_function_controller($params, $smarty)
{
  atkimport('atk.front.atkfrontcontroller');

  $parent = $smarty->__controller;
  $uri = $parent == NULL && isset($_REQUEST['uri']) ? $_REQUEST['uri'] : $params['uri'];
  $controller = atkFrontController::create($uri, $parent);  
  
  $smarty->__controller = $controller;

  if (is_object($parent))
    $request = $parent->getRequest()->toArray();
  else $request = $_REQUEST;
  
  unset($params['uri']);
  $request = array_merge($request, $params);
  
  $result = $controller->handle($request);
  
  $smarty->__controller = $controller->getParent();
  
  return $result;
}
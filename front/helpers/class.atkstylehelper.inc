<?php
class atkStyleHelper extends atkHelper
{
  /**
   * Stylesheet files.
   *
   * @var array
   */
  private $m_styleSheets = array();
  
  /**
   * Stylesheet code snippets.
   *
   * @var array
   */
  private $m_styleCodes = array();
  
  /**
   * Constructor.
   * 
   * @param atkFrontController $controller   controller instance
   * @param boolean            $autoRegister auto-register tags, blocks and other methods
   */
  public function __construct($controller, $autoRegister=true)
  {
    parent::__construct($controller, $autoRegister);
    $this->registerControllerStyleSheet();
  }
  
  /**
   * Register stylesheet for this controller if it exists.
   * 
   * Stylesheet will be searched for in the parent package it's styles/ 
   * directory. The stylesheet file should be named the same as the 
   * controller and should have a ".css" extension.
   */
  private function registerControllerStyleSheet()
  {
    $package = $this->getController()->getPackage();
    $package = substr($package, 0, strrpos($package, '.'));
    $path = getClassPath($package, false);    
    $filename = $path.'styles/'.$this->getController()->getName().'.css';
    
    if (file_exists($filename))
    {
      $this->registerStyleSheet($filename);
    }
  }
  
  /**
   * Style block handler. You can either supply the block with a src parameter
   * or you can provide your styles inside the block.
   *
   * @param array   $params  block parameters
   * @param string  $content block contents
   * @param boolean $repeat  will this block method be called again?
   */
  public function styleBlock($params, $content, &$repeat)
  {
    if ($repeat) return;
    
    if (@$params['src'])
    {
      $this->registerStyleSheet($params['src']);
    }
    else 
    {
      $this->registerStyleCode($content);
    }
  }


  /**
   * Register stylesheet of the given media type.
   *
   * @param string $file stylesheet filename
   * @param string $media media type (defaults to 'all')
   */
  public function registerStyleSheet($file, $media='all')
  {
    $this->m_styleSheets[] = array('file' => $file, 'media' => $media);
    $this->getController()->getBridge()->registerStyleSheet($file, $media);
  }

  /**
   * Register stylesheet code.
   *
   * @param string $code stylesheet code
   */
  public function registerStyleCode($code)
  {
    $this->m_styleCodes[] = $code;
    $this->getController()->getBridge()->registerStyleCode($code);    
  }  
  
  /**
   * Style for layout tag handler.
   * 
   * @param array tag parameters
   * 
   * @return string result
   */
  public function styleForLayoutTag($params)
  {
    $result = '';

    foreach ($this->m_styleSheets as $sheet)
    {
      $result .= '<link rel="stylesheet" type="text/css href="'.$sheet.'"></link>';
    }
  
    foreach ($this->m_styleCodes as $code)
    {
      $result .= '<style type="text/css">'.$code.'</style>';
    }

    return $result;
  }  
}
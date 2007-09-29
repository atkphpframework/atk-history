<?php
class atkScriptHelper extends atkHelper
{
  /**
   * Script files.
   *
   * @var array
   */
  private $m_scriptFiles = array();
  
  /**
   * Script code snippets.
   *
   * @var array
   */
  private $m_scriptCodes = array();
  
  /**
   * Constructor.
   * 
   * @param atkFrontController $controller   controller instance
   * @param boolean            $autoRegister auto-register tags, blocks and other methods
   */
  public function __construct($controller, $autoRegister=true)
  {
    parent::__construct($controller, $autoRegister);
    $this->registerControllerScriptFile();
  }
  
  /**
   * Register script for this controller if it exists.
   * 
   * Script will be searched for in the parent package it's scripts/ directory.
   * The script file should be named the same as the controller and should have
   * a ".js" extension.
   */
  private function registerControllerScriptFile()
  {
    $package = $this->getController()->getPackage();
    $package = substr($package, 0, strrpos($package, '.'));
    $path = getClassPath($package, false);    
    $filename = $path.'scripts/'.$this->getController()->getName().'.js';
    
    if (file_exists($filename))
    {
      $this->registerScriptFile($filename);
    }
  }
  
  /**
   * Script block handler. You can either supply the block with a src parameter 
   * or you can provide code inside the block.
   *
   * @param array   $params  block parameters
   * @param string  $content block contents
   * @param boolean $repeat  will this block method be called again?
   */
  public function scriptCodeBlock($params, $content, &$repeat)
  {
    if ($repeat) return;
    
    $this->registerScriptCode($content);
  }
  
  public function scriptFileTag($params)
  {
    $this->registerScriptFile($params['src']);
  }

  /**
   * Register script file.
   *
   * @param string $file script filename
   */
  public function registerScriptFile($file)
  {
    $this->m_scriptFiles[] = $file;
    $this->getController()->getBridge()->registerScriptFile($file);
  }

  /**
   * Register JavaScript code.
   *
   * @param string $code
   */
  public function registerScriptCode($code)
  {
    $this->m_scriptCodes[] = $code;
    $this->getController()->getBridge()->registerScriptCode($code);    
  }  
  
  /**
   * Script for layout tag handler.
   * 
   * @param array tag parameters
   * 
   * @return string result
   */
  public function scriptForLayoutTag($params)
  {
    $result = '';

    foreach ($this->m_scriptFiles as $file)
    {
      $result .= "<script type=\"text/javascript\" src=\"".htmlentities($file)."\"></script>\n";
    }

    foreach ($this->m_scriptCodes as $code)
    {
      $result .= "<script type=\"text/javascript\">\n{$code}\n</script>\n";
    }

    return $result;
  }
}
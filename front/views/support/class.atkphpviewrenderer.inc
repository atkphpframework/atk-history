<?php
class atkPHPFrontViewRenderer implements ArrayAccess 
{
  private $m_template;
  private $m_vars;
  private $m_tags;
  private $m_blocks;
  private $m_inline;

  private $m_blockStack = array();
    
  protected function __construct($template, &$vars, $tags, $blocks, $inline)
  {
    $this->m_template = $template;
    $this->m_vars = &$vars;
    $this->m_tags = array_combine(array_map('strtolower', array_keys($tags)), array_values($tags));
    $this->m_blocks = array_combine(array_map('strtolower', array_keys($blocks)), array_values($blocks));
    $this->m_inline = $inline;
  }
  
  public static function inline($template, &$vars, $tags, $blocks)
  {
    $renderer = new self($template, $vars, $tags, $blocks, true);
    return $renderer->render();
  }
  
  public static function file($filename, &$vars, $tags, $blocks)
  {
    $renderer = new self($filename, $vars, $tags, $blocks, false);
    return $renderer->render();
  }
  
  protected function render()
  {
    ob_start();
    
    if ($this->m_inline)
    {
      eval('?>'.$this->m_template.'<?php ');
    }
    else 
    {
      include($this->m_template);
    }
    
    if (count($this->m_blockStack) > 0)
    {
      throw new atkFrontException(atkFrontException::RENDER_ERROR, "Missing block close tag(s) (".implode(', ', array_reverse($this->m_blockStack)));
    }
    
    return ob_get_clean();
  }

  /**
   * Checks if a certain offset exists.
   *
   * @param mixed $offset offset
   * @return bool offset exists?
   */
  public function offsetExists($offset)
  {
    return isset($this->m_vars[$offset]);
  }

  /**
   * Returns the value at the given offset.
   *
   * @param mixed $offset
   * @return mixed value
   */
  public function offsetGet($offset)
  {
    return $this->m_vars[$offset];
  }

  /**
   * Sets the given offset with the given value
   *
   * @param mixed $offset offset
   * @param mixed $value  value
   */
  public function offsetSet($offset, $value)
  {
    $this->m_vars[$offset] = $value;
  }

  /**
   * Unsets the given offset.
   *
   * @param mixed $offset offset
   */
  public function offsetUnset($offset)
  {
    unset($this->m_vars[$offset]);
  } 
  
  /**
   * Handle block start.
   * 
   * @param string $block block name
   */
  private function handleBlockStart($block)
  {
    if (!isset($this->m_blocks[$block]))
    {
      throw new atkFrontException(atkFrontException::RENDER_ERROR, "Unknown block start tag '$block'");
    }
    
    ob_start();
    $this->m_blockStack[] = $block;    
    $handler = $this->m_blocks[$block];
    $repeat = true;
    call_user_func_array($handler, array($params, null, &$repeat));
    
    return true;
  }
  
  /**
   * Handle block end.
   * 
   * @param string $block block name
   */
  private function handleBlockEnd($block)
  {
    if (!isset($this->m_blocks[$block]))
    {
      throw new atkFrontException(atkFrontException::RENDER_ERROR, "Unknown block end tag '$block'");
    }
    
    $startBlock = array_pop($this->m_blockStack);
    if ($block != $startBlock)
    {
      throw new atkFrontException(atkFrontException::RENDER_ERROR, "Invalid block close tag '$block', expected '$blockStart'");
    }
  
    $content = ob_get_clean();
    $handler = $this->m_blocks[$block];
    $repeat = false;
    $result = call_user_func_array($handler, array($params, $content, &$repeat));

    echo $result;
    
    if ($repeat)
    {
      ob_start();
      $this->m_blockStack[] = $block;      
      return false;
    }
    
    return true;
  }
  
  /**
   * Handle tag.
   * 
   * @param string $tag tag name
   */
  private function handleTag($tag, $params)
  {
    if (!isset($this->m_tags[$tag]))
    {
      throw new atkFrontException(atkFrontException::RENDER_ERROR, "Unknown tag '$tag'");
    }
    
    $handler = $this->m_tags[$tag];
    $result = call_user_func_array($handler, array($params));
    
    echo $result;
    
    return true;
  }
  
  /**
   * View methods.
   */
  public function __call($method, $params)
  {
    $method = strtolower($method);
    $params = isset($params[0]) ? $params[0] : array();

    if (substr($method, -5) == 'start')
    {
      $block = substr($method, 0, -5);
      return $this->handleBlockStart($block, $params);
    }
    else if (substr($method, -3) == 'end')
    {
      $block = substr($method, 0, -3);
      return $this->handleBlockEnd($block, $params);
    }
    else 
    {
      $tag = $method;
      return $this->handleTag($tag, $params);
    }
  }
}
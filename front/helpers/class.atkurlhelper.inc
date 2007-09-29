<?php
class atkUrlHelper extends atkHelper
{
  /**
   * Smarty wrapper function for the url function.
   *
   * @param array $params parameters
   * 
   * @return string url
   */
  public function urlForTag($params)
  {
    return $this->getController()->url($params);
  }

  /**
   * Smarty wrapper function for the url function.
   *
   * @param array $params parameters
   * @param string $content content between link tags
   * @param boolean $repeat true first time
   * 
   * @return string link
   */
  public function linkToBlock($params, $content, &$repeat)
  {
    if (!$repeat)
    {
      return '<a href="'.$this->getController()->url($params).'">'.$content.'</a>';
    }
  }
}
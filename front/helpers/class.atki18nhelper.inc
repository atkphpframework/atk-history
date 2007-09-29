<?php
class atkI18NHelper extends atkHelper 
{
  private $m_language;
  
  public function i18nTag($params)
  {
    $string = isset($params['string']) ? $params['string'] : @$params[0];
    $language = isset($params['language']) ? $params['language'] : $this->getLanguage();
    $module = $this->getController()->getModule();
    return atktext($string, $module, null, $language);
  }
  
  public function setLanguage($language)
  {
    $this->m_language = $language;
  }
  
  public function getLanguage()
  {
    return $this->m_language;
  }
}
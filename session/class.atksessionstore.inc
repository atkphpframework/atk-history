<?php

class atkSessionStore
{
  private static $_instances = array();
  private $_key;

  /**
   *
   * @param unknown_type $reset
   * @return atkSessionStore
   */
  public static function getInstance($key=false, $reset=false)
  {
    if (!$key) $key = self::getKeyFromSession();
    if (!isset(self::$_instances[$key]) || $reset) self::$_instances[$key] = new self($key);
    return self::$_instances[$key];
  }

  private static function getKeyFromSession()
  {
    $sessionmanager = self::getSessionManager();
    if (!$sessionmanager) return false;
    else                  return $sessionmanager->globalStackVar("atkstore_key");
  }

  private function __construct($key)
  {
    $this->_key = $key;
  }

  public function getKey() { return $this->_key; }

  public function addDataRow($row, $primary_key_field)
  {
    $data = $this->getData();
    if ($data===false) return false;

    $primary_key = -1 * count($data);
    $row[$primary_key_field] = $primary_key;
    $data[] = $row;

    $this->setData($data);

    return $primary_key;
  }

  public function getDataRowForSelector($selector)
  {
    $data = $this->getData();
    if (!$data) return false;

    $row_key = self::getRowKeyFromSelector($selector);
    if ($row_key===false) return false;

    return $data[$row_key];
  }

  public function updateDataRowForSelector($selector, $row)
  {
    $data = $this->getData();
    if (!$data) return false;

    $row_key = self::getRowKeyFromSelector($selector);
    if ($row_key===false) return false;

    $data[$row_key] = $row;

    $this->setData($data);
    return $row;
  }

  public function deleteDataRowForSelector($selector)
  {
    $data = $this->getData();
    if (!$data) return false;

    $row_key = self::getRowKeyFromSelector($selector);
    if ($row_key===false) return false;

    atkdebug("Unsetting $row_key");
    unset($data[$row_key]);
    atk_var_dump($data, "new data");

    $this->setData($data);
    return true;
  }

  protected static function getSessionManager()
  {
    $sessionmanager = atkGetSessionManager();
    if (!$sessionmanager) return false;
    else                  return $sessionmanager;
  }

  public function getData()
  {
    if (!$this->_key) return false;

    $sessionmanager = self::getSessionManager();
    if (!$sessionmanager) return false;

    $data = $sessionmanager->globalStackVar($this->_key);
    if (!is_array($data)) $data = array();
    atk_var_dump($data,"data from session");
    return $data;
  }

  private function setData($data)
  {
    if (!$this->_key) return false;

    $sessionmanager = self::getSessionManager();
    if (!$sessionmanager) return false;

    $sessionmanager->globalStackVar($this->_key, $data);
    return $data;
  }

  /**
   * We sneak rowkeys in the selectors as negative ids.
   *
   * @param unknown_type $selector
   * @return unknown
   */
  private static function getRowKeyFromSelector($selector)
  {
    $selector = decodeKeyValuePair($selector);
    $selector_values = array_values($selector);
    atk_var_dump($selector_values,"selectorvalues ".count($selector_values)."===1 && ".is_int($selector_values[0]).". && {$selector_values[0]}<=0");
    if (count($selector_values)===1 && is_numeric($selector_values[0]) && $selector_values[0]<=0)
    {
      return -1*$selector_values[0];
    }
    return false;
  }
}
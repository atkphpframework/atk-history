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
    atk_var_dump($row, __CLASS__.'->'.__METHOD__.": Adding a new row to session store with primary key field '$primary_key_field' and key: ".$this->getKey());
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
    atk_var_dump($selector, __CLASS__.'->'.__METHOD__.": Getting row from session store with key: ".$this->getKey());
    $data = $this->getData();
    if (!$data) return false;

    $row_key = self::getRowKeyFromSelector($selector);
    if ($row_key===false) return false;

    return $data[$row_key];
  }

  public function updateDataRowForSelector($selector, $row)
  {
    atk_var_dump($row, __CLASS__.'->'.__METHOD__.": Updating row in session store with key: ".$this->getKey()." and selector: $selector");
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
    atk_var_dump($selector, __CLASS__.'->'.__METHOD__.": Deleting row from session store with key: ".$this->getKey());
    $data = $this->getData();
    if (!$data) return false;

    $row_key = self::getRowKeyFromSelector($selector);
    if ($row_key===false) return false;

    unset($data[$row_key]);

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

    if (count($selector_values)===1 && is_numeric($selector_values[0]) && $selector_values[0]<=0)
    {
      return -1*$selector_values[0];
    }
    return false;
  }
}
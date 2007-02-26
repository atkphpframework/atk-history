<?php
/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_Feed
 * @copyright  Copyright (c) 2006 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */


/** Zend_Feed */
require_once 'Zend/Feed.php';

/** Zend_Feed_Element */
require_once 'Zend/Feed/Element.php';


/**
 * Zend_Feed_EntryAbstract represents a single entry in an Atom or RSS
 * feed.
 *
 * @category   Zend
 * @package    Zend_Feed
 * @copyright  Copyright (c) 2006 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
abstract class Zend_Feed_EntryAbstract extends Zend_Feed_Element
{
    /**
     * Root XML element for entries. Subclasses must define this to a
     * non-null value.
     *
     * @var string
     */
    protected $_rootElement;

    /**
     * Root namespace for entries. Subclasses may define this to a
     * non-null value.
     *
     * @var string
     */
    protected $_rootNamespace = null;


    /**
     * Zend_Feed_EntryAbstract constructor
     *
     * The Zend_Feed_EntryAbstract constructor takes the URI of the feed the entry
     * is part of, and optionally an XML construct (usually a
     * SimpleXMLElement, but it can be an XML string or a DOMNode as
     * well) that contains the contents of the entry.
     */
    public function __construct($uri = null, $element = null)
    {
        if (!($element instanceof DOMElement)) {
            if ($element) {
                // Load the feed as an XML DOMDocument object
                @ini_set('track_errors', 1);
                $doc = new DOMDocument();
                $success = @$doc->loadXML($element);
                @ini_restore('track_errors');

                if (!$success) {
                    throw new Zend_Feed_Exception("DOMDocument cannot parse XML: $php_errormsg");
                }

                $element = $doc->getElementsByTagName($this->_rootElement)->item(0);
                if (!$element) {
                    throw new Zend_Feed_Exception('No root <' . $this->_rootElement . '> element found, cannot parse feed.');
                }
            } else {
                $doc = new DOMDocument('1.0', 'utf-8');
                if ($this->_rootNamespace !== null) {
                    $element = $doc->createElementNS(Zend_Feed::lookupNamespace($this->_rootNamespace), $this->_rootElement);
                } else {
                    $element = $doc->createElement($this->_rootElement);
                }
            }
        }

        parent::__construct($element);
    }

}

<?php
/**
 * @copyright Copyright (c) 2010, The volkszaehler.org project
 * @package default
 * @license http://www.opensource.org/licenses/gpl-license.php GNU Public License
 */
/*
 * This file is part of volkzaehler.org
 *
 * volkzaehler.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * volkzaehler.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with volkszaehler.org. If not, see <http://www.gnu.org/licenses/>.
 */

namespace Volkszaehler\Model;

use Volkszaehler\Util;
use Volkszaehler\Model;

/**
 * Property entity
 *
 * @author Steffen Vogel <info@steffenvogel.de>
 * @package default
 *
 * @Entity
 * @Table(
 * 		name="properties",
 * 		uniqueConstraints={
 * 			@UniqueConstraint(name="unique_properties", columns={"id", "name"})
 * 		}
 * )
 * @HasLifecycleCallbacks
 */
class Property {
	/**
	 * @Id
	 * @Column(type="smallint", nullable=false)
	 * @GeneratedValue(strategy="AUTO")
	 *
	 * @todo wait until DDC-117 is fixed (PKs on FKs)
	 */
	protected $id;

	/** @Column(type="string", nullable=false) */
	protected $name;

	/** @Column(type="string", nullable=false) */
	protected $value;

	/** @ManyToOne(targetEntity="Entity", inversedBy="properties") */
	protected $entity;

	/**
	 * Constructor
	 *
	 * @param string $key
	 * @param string $value
	 */
	public function __construct(Model\Entity $entity, $name, $value) {
		$this->setName($name);
		$this->setValue($value);

		$this->entity = $entity;
	}

	/**
	 * Cast property value according to $this->type
	 *
	 * @PostLoad
	 */
	public function castValue() {
		if ($this->getDefinition()->getType() != 'multiple') {
			settype($this->value, $this->getDefinition()->getType());
		}
	}

	/**
	 * Validate property name & value
	 *
	 * Throws an exception if something is incorrect
	 *
	 * @PrePersist
	 * @PreUpdate
	 */
	public function validate() {
		if (!PropertyDefinition::exists($this->name)) {
			throw new \Exception('invalid property name: ' . $this->name);
		}

		if (!$this->getDefinition()->validateValue($this->value)) {
			throw new \Exception('invalid property value: ' . $this->value);
		}
	}

	/*
	 * Setter & Getter
	 */
	public function getName() { return $this->name; }
	public function getValue() { return $this->value; }
	public function getDefinition() { return PropertyDefinition::get($this->name); }

	public function setValue($value) { $this->value = $value; }
	protected function setName($name) { $this->name = $name; }
}

?>
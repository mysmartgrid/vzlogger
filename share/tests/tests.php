<?php
/*
 * Copyright (c) 2010 by Justin Otherguy <justin@justinotherguy.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (either version 2 or
 * version 3) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

include '../../backend/init.php';

$channel = current(Channel::getByType('PowerMeter'));

$data = $channel->getData(1270062000000, 1270666800000, 400);

echo '<table border="1">';
foreach ($data as $i => $reading) {
	echo '<tr><td>' . ($i + 1) . '</td><td>' . date('h:i:s', $reading['timestamp']/1000) . '</td><td>' . $reading['value'] . '</td><td>' . $reading['count'] . '</td></tr>';
}
echo '</table>';

echo '<pre>';
var_dump(Database::getConnection());
echo '</pre>';

?>
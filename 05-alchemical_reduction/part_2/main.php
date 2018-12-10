<?php

function invert($str) {
    $unit = ord($str);

    if ($unit >= 65 && $unit <= 90) {
        return strtolower(chr($unit));
    } else if ($unit >= 97 && $unit <= 122) {
        return strtoupper(chr($unit));
    }
}

$input = fopen('input.txt', 'r');
$content = fread($input, filesize('input.txt'));

$problemUnits = array();

for ($c = 65; $c <= 90; $c++) {
    $reductionResult = [];

    for ($i = 0; $i < strlen($content); $i++) {
        $unit = $content[$i];

        if (strtoupper($unit) == chr($c) || ord($unit) < 65) {
            continue;
        }

        if (invert(end($reductionResult)) == $unit) {
            array_pop($reductionResult);
        } else {
            $reductionResult[] = $unit;
        }
    }

    $problemUnits[$c] = count($reductionResult);
}

$minReduction = 999999999999999;
$problemUnit;

foreach ($problemUnits as $unit => $reduction) {
    if ($reduction < $minReduction) {
        $problemUnit = $unit;
        $minReduction = $reduction;
    }
}

echo "Problem unit: " . chr($problemUnit) . ", after reduction: " . $minReduction . " units.\n";

?>

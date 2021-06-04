<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.0/dist/css/bootstrap.min.css">
</head>
<body>
<h1 align="center">Alocator de memorie</h1>

<?php
error_reporting(E_ERROR | E_PARSE);
exec("Alocator.exe");
$First = fopen("outputFirst.txt", "r") or die("Unable to open file!");
// Output one line until end-of-file
$dataFirst = array();
$i = 0;
while(!feof($First)) {
    $s = fgets($First);
    if($s != "") {
        $pos = stripos($s, " ");
        $dataFirst[$i]['x'] = round(intval(substr($s, 0, $pos)) );
        $dataFirst[$i]['y'] = round(intval(substr($s, $pos + 1, strlen($s) - $pos)));
        $i++;
    }
}
unset($dataFirst[end($dataFirst)]);
fclose($First);

$Best = fopen("outputBest.txt", "r") or die("Unable to open file!");
// Output one line until end-of-file
$dataBest = array();
$i = 0;
while(!feof($Best)) {
    $s = fgets($Best);
    if($s != "") {
        $pos = stripos($s, " ");
        $dataBest[$i]['x'] = round(intval(substr($s, 0, $pos)) );
        $dataBest[$i]['y'] = round(intval(substr($s, $pos + 1, strlen($s) - $pos)));
        $i++;
    }
}
unset($dataBest[end($dataBest)]);
fclose($Best);

?>

<?php
$input = fopen("input.in", "r") or die("Unable to open file!");
// Output one line until end-of-file
echo "Durata simularii ".fgets($input)."<br>";
echo "Memoria Maxima ".fgets($input)."<br>";
echo "Durata minima dintre procese ".fgets($input)."<br>";
echo "Durata maxima dintre procese ".fgets($input)."<br>";
echo "Memoria minima  ".fgets($input)."<br>";
echo "Memoria maxima ".fgets($input)."<br>";
echo "Durata minima a procesului ".fgets($input)."<br>";
echo "Durata maxima a procesului ".fgets($input)."<br>";

fclose($input);
?>
<script src="https://canvasjs.com/assets/script/canvasjs.min.js"></script>
<script type="text/javascript">
    window.onload = function () {
        var data1 = [];
        var dataSeries1 = { type: "line" };
        var dataPoints1 = <?php echo json_encode($dataBest); ?>;
        dataSeries1.dataPoints = dataPoints1;
        data1.push(dataSeries1);

        var data = [];
        var dataSeries = { type: "line" };
        var dataPoints = <?php echo json_encode($dataFirst); ?>;

        dataSeries.dataPoints = dataPoints;
        data.push(dataSeries);

        var chart = new CanvasJS.Chart("chartContainer1", {
            animationEnabled: true,
            zoomEnabled: true,
            title:{
                text: "First Fit"
            },
            data: data  // random generator below
        });




    var chart1 = new CanvasJS.Chart("chartContainer2", {
        animationEnabled: true,
        zoomEnabled: true,
        title:{
            text: "Best Fit"
        },
        data: data1  // random generator below
    });

        chart.render();
        chart1.render();
        console.log(data)
        console.log(data1)
    };

</script>
<div id="chartContainer1" style="height: 300px; width: 100%;"></div>

<?php
$aditionalFirst = fopen("dateAditionaleFirst.txt", "r") or die("Unable to open file!");
// Output one line until end-of-file
echo "Numarul de procese ".fgets($aditionalFirst)."<br>";
echo "Numarul de procese alocate cu succes ".fgets($aditionalFirst)."<br>";
echo "Numarul de procese care nu au putut fi alocate ".fgets($aditionalFirst)."<br>";

fclose($aditionalFirst);
?>

<div id="chartContainer2" style="height: 300px; width: 100%;"></div>

<?php
$aditionalBest = fopen("dateAditionaleBest.txt", "r") or die("Unable to open file!");
// Output one line until end-of-file
echo "Numarul de procese ".fgets($aditionalBest)."<br>";
echo "Numarul de procese alocate cu succes ".fgets($aditionalBest)."<br>";
echo "Numarul de procese care nu au putut fi alocate ".fgets($aditionalBest)."<br>";

fclose($aditionalBest);
?>

</body>
</html>

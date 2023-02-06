<?php
    $dbname = 'iotprojek';
    $dbuser = 'root';  
    $dbpass = ''; 
    $dbhost = 'localhost';

    $connect = mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

    $Temperature = $_GET['temperature'];
    $Humidity = $_GET['kelembapan'];
    $sensorapi = $_GET['sensorapi'];

    mysqli_query($connect, "ALTER TABLE sensor_data AUTO_INCREMENT=1");
    $query = "INSERT INTO sensor_data (temperature, humidity, sensorapi) VALUES ('$Temperature', '$Humidity', '$sensorapi')";
    $result = mysqli_query($connect, $query);

    if($result){
        echo "berhasil";
    } else{
        echo "gagal";
    }
?>
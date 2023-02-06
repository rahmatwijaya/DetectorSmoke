<?php
    $dbname = 'notifikasi';
    $dbuser = 'root';  
    $dbpass = ''; 
    $dbhost = 'localhost';

    $connect = mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

    $judul = $_GET["judul"];
    $isi = $_GET["isi"];

    mysqli_query($connect, "ALTER TABLE data_notif AUTO_INCREMENT=1");
    $query = "INSERT INTO data_notif (judul, isi) VALUES ('$judul', '$isi')";
    $result = mysqli_query($connect, $query);

    $url = "https://fcm.googleapis.com/fcm/send";
    $token = "eV4MevGxTqSQSFQrXjkjgm:APA91bFFAze1CE36BvlkkfFOTT49XCa7WGjYxglidTAxS__Lb2aaASVKe5fSM1WqsoFISCN7Mr45aUFkL6pLCAr_CTTks6sBBruNzqMEh2ts9d6wVhYpXertuUT0iyRnIpy_DqD-lFT0";
    $serverKey = 'AAAAh9kpSJ4:APA91bHLryozve7L0ntyuvwG_Q97_muQx4ovGAdN2d4OVV2kEaVXr2fy1CgmXa2YsoeAKO_cZBrTTGe7gwDYB-6pw3_G4ZQX40_Orqa_8KdHqiUGdU-z0fQ5UgkJbThA9Cm8Ws9Lg3HA';
    $title = $judul;
    $body = $isi;
    $notification = array('title' =>$title , 'body' => $body, 'sound' => 'default', 'badge' => '1', 'route' => '/fcm-page');
    $arrayToSend = array('to' => $token, 'notification' => $notification,'priority'=>'high');
    $json = json_encode($arrayToSend);
    $headers = array();
    $headers[] = 'Content-Type: application/json';
    $headers[] = 'Authorization: key='. $serverKey;
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST,"POST");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $json);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    //Send the request
    $response = curl_exec($ch);
    //Close request
    if ($response === FALSE) {
    die('FCM Send Error: ' . curl_error($ch));
    }
    curl_close($ch);
?>
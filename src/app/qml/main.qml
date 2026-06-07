import QtQuick
import QtQuick.Controls
import QtCharts
import Spectrometer

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Column {
        anchors.fill: parent

        // ChartView {
        //     id: chart
        //     width: parent.width
        //     height: parent.height - 50   // leave space for button

        //     ValueAxis { id: axisX; min: 400; max: 700 }
        //     ValueAxis { id: axisY; min: 0; max: 2 }

        //     LineSeries {
        //         id: series
        //         axisX: axisX
        //         axisY: axisY
        //     }
        // }

        SpectrumView {
                id: spectrumView
                // anchors.fill: parent
                visible: true
                width: parent.width
                height: parent.height - 50   // leave space for button
            }

        Button {
            text: "Start"
            height: 50
            onClicked: controller.start()
        }
    }

    // Connections {
    //     target: controller

    //     function onSpectrumUpdated(wl, intensity) {
    //         series.clear()
    //         for (let i = 0; i < wl.length; i++) {
    //             series.append(wl[i], intensity[i])
    //         }
    //     }
    // }



    Connections {
        target: controller

        function onSpectrumUpdated(wl, intensity) {
            console.log("QML received:", wl.length)
            spectrumView.setSpectrum(wl, intensity)
        }
    }
}


// import QtQuick
// import QtQuick.Controls
// import QtCharts

// ApplicationWindow {
//     visible: true
//     width: 800
//     height: 600

//     Column {
//         anchors.fill: parent

//         ChartView {
//             id: chart
//             width: parent.width
//             height: parent.height - 50

//             ValueAxis { id: axisX; min: 400; max: 700 }
//             ValueAxis { id: axisY; min: 0; max: 2 }

//             LineSeries {
//                 id: series
//                 axisX: axisX
//                 axisY: axisY
//             }
//         }

//         Button {
//             text: "Start"
//             height: 50
//             onClicked: controller.start()
//         }
//     }

//     Connections {
//         target: controller

//         function onSpectrumUpdated(wl, intensity) {
//             series.clear()
//             for (let i = 0; i < wl.length; i++) {
//                 series.append(wl[i], intensity[i])
//             }
//         }
//     }
// }

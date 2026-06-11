import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Spectrometer

ApplicationWindow {
    id: root
    visible: true
    width: 1100
    height: 720
    title: "Spectrometer Analysis"

    property string statusText: controller.running ? "Acquiring" : "Idle"
    property string errorText: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 112
            color: "#f5f6f8"
            border.color: "#d9dde3"

            GridLayout {
                anchors.fill: parent
                anchors.margins: 12
                columns: 8
                columnSpacing: 10
                rowSpacing: 8

                Label {
                    text: "Source"
                    font.pixelSize: 12
                    color: "#4b5563"
                }

                ComboBox {
                    id: sourceControl
                    Layout.preferredWidth: 150
                    model: ["Synthetic", "File replay"]
                    currentIndex: controller.acquisitionSource
                    onActivated: controller.setAcquisitionSource(currentIndex)
                }

                Label {
                    text: "Profile"
                    font.pixelSize: 12
                    color: "#4b5563"
                }

                ComboBox {
                    id: profileControl
                    Layout.preferredWidth: 180
                    enabled: sourceControl.currentIndex === 0
                    model: ["Emission standard", "Absorbance dye", "Polymer blend", "Dark frame", "Reference lamp"]
                    currentIndex: controller.syntheticProfile
                    onActivated: controller.setSyntheticProfile(currentIndex)
                }

                Label {
                    text: "Mode"
                    font.pixelSize: 12
                    color: "#4b5563"
                }

                ComboBox {
                    id: domainControl
                    Layout.preferredWidth: 120
                    enabled: sourceControl.currentIndex === 0
                    model: ["Raw", "Dark", "Reference"]
                    currentIndex: controller.spectrumDomain
                    onActivated: controller.setSpectrumDomain(currentIndex)
                }

                Button {
                    Layout.preferredWidth: 96
                    text: controller.running ? "Stop" : "Start"
                    onClicked: controller.running ? controller.stop() : controller.start()
                }

                Label {
                    text: root.statusText
                    font.pixelSize: 12
                    color: controller.running ? "#047857" : "#4b5563"
                }

                Label {
                    text: "Seed"
                    font.pixelSize: 12
                    color: "#4b5563"
                }

                SpinBox {
                    id: seedControl
                    Layout.preferredWidth: 150
                    enabled: deterministicControl.checked && sourceControl.currentIndex === 0
                    from: 1
                    to: 999999
                    value: controller.seed
                    editable: true
                    onValueModified: controller.setSeed(value)
                }

                Label {
                    text: "Deterministic"
                    font.pixelSize: 12
                    color: "#4b5563"
                }

                Switch {
                    id: deterministicControl
                    checked: controller.deterministic
                    enabled: sourceControl.currentIndex === 0
                    onToggled: controller.setDeterministic(checked)
                }

                Label {
                    text: "Replay CSV"
                    font.pixelSize: 12
                    color: "#4b5563"
                }

                TextField {
                    id: replayPathControl
                    Layout.columnSpan: 3
                    Layout.fillWidth: true
                    enabled: sourceControl.currentIndex === 1
                    text: controller.replayFilePath
                    placeholderText: "data/synthetic/emission_standard.csv"
                    selectByMouse: true
                    onEditingFinished: controller.setReplayFilePath(text)
                }

                Label {
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                    text: root.errorText
                    font.pixelSize: 12
                    color: "#b91c1c"
                }
            }
        }

        SpectrumView {
            id: spectrumView
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: true
        }
    }

    Connections {
        target: controller

        function onSpectrumUpdated(wl, intensity) {
            spectrumView.setSpectrum(wl, intensity)
            root.errorText = ""
        }

        function onErrorChanged(message) {
            root.errorText = message
        }

        function onConfigChanged() {
            sourceControl.currentIndex = controller.acquisitionSource
            profileControl.currentIndex = controller.syntheticProfile
            domainControl.currentIndex = controller.spectrumDomain
            deterministicControl.checked = controller.deterministic
            seedControl.value = controller.seed
            replayPathControl.text = controller.replayFilePath
        }
    }
}

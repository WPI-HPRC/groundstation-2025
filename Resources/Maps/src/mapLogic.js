const maps = {
    spaceport: {
        name: "spaceport",
        center: [32.99020169835385, -106.97596734602624],
        minZoom: 11,
        maxZoom: 13
    },
    albans: {
        name: "albans",
        center: [44.823991, -73.165806],
        minZoom: 13,
        maxZoom: 15
    },
    wpi: {
        name: "wpi",
        center: [42.281849, -71.817541],
        minZoom: 13,
        maxZoom: 15
    },
}

for (const [mapName, mapInfo] of Object.entries(maps)) {
    mapInfo["mapTiles"] = L.tileLayer(`qrc:/Maps/Maps/${mapInfo.name}/{z}/{x}/{y}.jpg`, { maxZoom: mapInfo.maxZoom, minZoom: mapInfo.minZoom })
}

let currentMap;
let map = L.map("map").setView([0,0], 1)

// Create a line tracking the path of the payload
const pathConfig = {
    color: "#2222ff",
    // dashArray: [11,11],
    opacity: 0.6,
    smoothFactor: 1.0,
}
let path;

// Create the payload marker
const payloadConfig = {
    color: "#2222ff",
    radius: 10,
    fillOpacity: 0.3
}
const payloadPosition = [0, 0]
const payload = L.circleMarker(payloadPosition, payloadConfig)
payload.addTo(map)

reset()

function runPathTest() {
    setInterval(() => {
        const point = payload.getLatLng()
        path.addLatLng(point)
        const rand1 = Math.random() * 2 - 1
        const rand2 = Math.random() * 2 - 1
        payload.setLatLng(L.latLng(point.lat + 0.005 * rand1, point.lng + 0.005 * rand2))
    }, 200)
}

function setMap(name) {
    if(currentMap) {
        currentMap.mapTiles.remove()
    }
    currentMap = maps[name]
    currentMap.mapTiles.addTo(map)
    map.setView(currentMap.center, currentMap.maxZoom)
}

function addPayloadPoint(lat, lng) {
    // So we don't draw a point at (0, 0)
    if(payload.getLatLng().lat === 0) {
        payload.setLatLng(L.latLng(lat, lng))
        return
    }

    const currentPoint = payload.getLatLng()

    // Add the last payload point to the path, and update it
    path.addLatLng(currentPoint)

    const latLng = L.latLng(lat, lng)
    payload.setLatLng(latLng)
}

function reset() {
    payload.setLatLng([0, 0])
    if(path) {
        path.remove()
    }
    path = L.polyline([], pathConfig)
    path.addTo(map)
}

function centerMap(lat, lng) {
    if (map.getCenter().lat != lat && map.getCenter().lng != lng) {
        map.setView([lat, lng], map.zoom)
    }
}

// Initialize QT connection
if (typeof qt != 'undefined') new QWebChannel(qt.webChannelTransport, (channel) => {
    window.qtLeaflet = channel.objects.qtLeaflet

    // Connect to the payload point signal
    qtLeaflet.updatePayloadPoint.connect(function (latitude, longitude) {
        addPayloadPoint(latitude, longitude)
    })
    qtLeaflet.setMap.connect((name) => {
        setMap(name)
    })
    qtLeaflet.reset.connect(() => {
        reset()
    });
})
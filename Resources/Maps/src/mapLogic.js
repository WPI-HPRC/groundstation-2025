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
let path;

let markers = {
    rocket: {
        pathConfig: {
            color: "#AF283A",
            smoothFactor: 1.0,
            opacity: 0.6,
        },
        markerConfig: {
            color: "#AF283A",
            radius: 10,
            fillOpacity: 0.3
        },
        marker: null,
        path: null
    },
    payload: {
        pathConfig: {
            color: "#3A28AF",
            smoothFactor: 1.0,
            opacity: 0.6,
        },
        markerConfig: {
            color: "#3A28AF",
            radius: 10,
            fillOpacity: 0.3
        },
        marker: null,
        path: null
    }
}

markers.rocket.marker = L.circleMarker([0, 0], markers.rocket.markerConfig)
markers.payload.marker = L.circleMarker([0, 0], markers.payload.markerConfig)

markers.rocket.marker.addTo(map)
markers.payload.marker.addTo(map)

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

function addPoint(name, lat, lng) {
    let marker = markers[name]
    if(!marker) {
        return
    }

    // So we don't draw a point at (0, 0)
    if(marker.marker.getLatLng().lat === 0) {
        marker.marker.setLatLng(L.latLng(lat, lng))
        return
    }

    const currentPoint = marker.marker.getLatLng()

    // Add the last payload point to the path, and update it
    marker.path.addLatLng(currentPoint)

    const latLng = L.latLng(lat, lng)
    marker.marker.setLatLng(latLng)
}

function reset() {
    for (const [markerName, marker] of Object.entries(markers)) {
        marker.marker.setLatLng([0, 0])
        if(marker.path) {
            marker.path.remove()
        }
        marker.path = L.polyline([], marker.pathConfig)
        marker.path.addTo(map)
    }
}

function autoChooseMap()
{
    let chosenMapName = null
    for (const [markerName, marker] of Object.entries(markers)) {
        // Find the first marker with a GPS coordinate
        if(marker.marker.getLatLng().lat === 0) {
            continue
        }



        let minDistSquared = Infinity
        chosenMapName = Object.entries(maps)[0][0]
        for (const [mapName, mapInfo] of Object.entries(maps)) {
            /*
            qtLeaflet.log(`"Map Center[0]: ${mapInfo.center[0]}`);
            qtLeaflet.log(`"Map Center[1]: ${mapInfo.center[1]}`);
            qtLeaflet.log(`Rocket lat: ${marker.marker.getLatLng().lat}`)
            qtLeaflet.log(`Rocket long: ${marker.marker.getLatLng().lng}`)
*/
            const latDist = Math.pow(mapInfo.center[0] - marker.marker.getLatLng().lat, 2);
            const longDist = Math.pow(mapInfo.center[1] - marker.marker.getLatLng().lng, 2);
            let distSquared = latDist + longDist;
            if(distSquared < minDistSquared) {
                minDistSquared = distSquared
                chosenMapName = mapInfo.name
            }
            qtLeaflet.log(distSquared)
        }
    }
    if(chosenMapName) {
        setMap(chosenMapName)
        qtLeaflet.mapChosen(chosenMapName)
    }
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
    qtLeaflet.addPoint.connect((name, latitude, longitude) => {
        addPoint(name, latitude, longitude)
    })
    qtLeaflet.setMap.connect((name) => {
        setMap(name)
    })
    qtLeaflet.reset.connect(() => {
        reset()
    });
    qtLeaflet.autoChooseMap.connect(() => {
        autoChooseMap()
    })
})
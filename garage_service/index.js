var mqtt = require('mqtt')
var express = require('express')
var path = require('path')
const app = express()

var words = 'unknown'

var mqttUser = ''
var mqttPass = ''
var mqttServer = ''
var topic = 'garage'

var client  = mqtt.connect(mqttServer,
{
    username: mqttUser,
    password: mqttPass
}) 

app.set("view engine", "pug")
app.set("views", path.join(__dirname, "views"))


client.on('connect', function ()
{
  client.subscribe(topic)
})

client.on('message', function(topic, message)
{
    console.log(message.toString())
    if(message.toString() == 'o')
    {
        words = 'OPEN'
    }
    else
    {
        words = 'CLOSED'
    }
})

app.get('/', function(req, res)
{
    console.log(words)
    res.render("homepage", {
        message: words,
    })
})

app.listen(3000, function()
{
console.log('Listening on port 3000')
})

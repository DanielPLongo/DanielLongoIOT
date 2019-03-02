module.exports = function(RED) {
  function CelstoFahrNode(config) {
    RED.nodes.createNode(this, config);
    var node = this;
    node.on('input', function(msg) {
      msg.payload = (msg.payload * 1.8) + 32;
      node.send(msg);
    });
  }
  RED.nodes.registerType("celstofahr", CelstoFahrNode);
}
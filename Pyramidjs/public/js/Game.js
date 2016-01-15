var Game = function(json){
	this.client;
	this.players = {};
	this.map = null;

	this.endTime = null;

	this.online = false;

	this.init(json);
}

Game.prototype.init = function(json){
	for(var i in json){
		this[i] = json[i];
	}
}

Game.prototype.start = function(){
	var index = 0;
	if(this.map != null){
		for(var i = 0; i < maps.length; i++){
			if(maps[i].id == this.map.id){
				index = i+1;
				break;
			}
		}
		if(index >= maps.length){
			index = 0;
		}
	}
	this.map = new Map(maps[index]);
	this.endTime = Date.now() + 3 * 60 * 1000;
	var ps = this.players;
	this.players = {};
	for(var i in ps){
		this.addPlayer(ps[i])
	}
}

Game.prototype.update = function(){
	var now = Date.now();
	if(isServer){
        //En ligne
        var snapshot = this.getSnapshot();
        for(var i in this.players){
        	Utils.messageTo(this.players[i].socket, "snapshot", snapshot);
        }
        if(Date.now() > this.endTime){
        	this.start();
        }
    }else{
        //Client
        for(var i in this.players){
        	if(this.players[i].id == this.client.pID){
            	//moi
            	this.players[i].inputs = this.client.keys;
            	this.players[i].update();
            	if(this.online){
            		socket.emit("position", this.players[i].getSnapshotInfo());
            	}
            }else{
            	this.players[i].interpolate(now);
            }
        }
    }

}

Game.prototype.addPlayer = function(p){
	var toAdd = true;
	for(var i in this.players){
		if(this.players[i].id == p.id){
			toAdd = false;
			break;
		}
	}
	if(isServer){
		if(toAdd){
			for(var i in this.players){
				Utils.messageTo(this.players[i].socket, "newPlayer", p.getInitInfo());
			}
			p.spawn(this.map);
			this.players[p.socket] = p;
			Utils.messageTo(p.socket, "init", this.getInit());
		}
	}else{
		if(toAdd){
			this.players[p.id] = p;
		}
	}
}

Game.prototype.deletePlayer = function(p){
	var del = false;
	for(var i in this.players){
		if(this.players[i].id == p.id){
			del = true;
			if(isServer){
				delete this.players[p.socket];
			}else{
				delete this.players[p.id];
			}
		}
	}
	if(isServer){
		if(del){
			for(var i in this.players){
				Utils.messageTo(this.players[i].socket, "deletePlayer", {id:p.id});
			}
		}
	}
	return del;
}

Game.prototype.getPlayerBySocket = function(socket){
	if(this.players[socket]){
		return this.players[socket];
	}
	return null;
}

Game.prototype.getPlayerById = function(id){
	for(var i in this.players){
		if(this.players[i].id == id){
			return this.players[i];
		}
	}
	return null;
}

Game.prototype.getNbPlayers = function(){
	return Object.keys(this.players).length;
}

Game.prototype.getSnapshot = function(){
	var data = {};
	data.players = [];
	for(var i in this.players){
		data.players.push(this.players[i].getSnapshotInfo());
	}
	return data;
}

Game.prototype.getInit = function(){
	var data = {};
	data.timeLeft = this.endTime - Date.now();
	data.map = this.map.getInitInfos();
	data.players = [];
	for(var i in this.players){
		data.players.push(this.players[i].getInitInfo());
	}
	return data;
}
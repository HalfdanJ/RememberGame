#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    for(int i=0;i<5;i++){
        Tile tile; 
        tile.image.loadImage("details_0"+ofToString(i)+".png");
        tile.collisionImage.loadImage("col_0"+ofToString(i)+".png");
        tile.id = i;
        tiles.push_back(tile);
    }
    
    tiles[0].neighbors[0] = -1;
    tiles[0].neighbors[1] = 1;
    tiles[0].neighbors[2] = 3;
    tiles[0].neighbors[3] = -1;
    
    tiles[1].neighbors[0] = -1;
    tiles[1].neighbors[1] = 2;
    tiles[1].neighbors[2] = -1;
    tiles[1].neighbors[3] = 0;
    
    tiles[2].neighbors[0] = -1;
    tiles[2].neighbors[1] = 3;
    tiles[2].neighbors[2] = 4;
    tiles[2].neighbors[3] = 1;
    
    tiles[3].neighbors[0] = 0;
    tiles[3].neighbors[1] = 4;
    tiles[3].neighbors[2] = -1;
    tiles[3].neighbors[3] = 2;
    
    tiles[4].neighbors[0] = 2;
    tiles[4].neighbors[1] = -1;
    tiles[4].neighbors[2] = -1;
    tiles[4].neighbors[3] = 3;
    
    player.position = ofPoint(0.5,0.5);
    player.tile = 3;
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
    //Update player;
    
    //Gravity
    // player.v += ofxVec2f(0,0.01);
    
    //Keyforce
    player.v += keyForce;
    
    //Damping
    player.v *= 0.9;
    
    Player oldPlayer = player;
    
    player.position += player.v * 1.0/ofGetFrameRate();
    
    
    if(player.position.x > 1 && tiles[player.tile].neighbors[1] != -1){
        player.position.x -= 1;
        player.tile = tiles[player.tile].neighbors[1];
    }
    if(player.position.x < 0 && tiles[player.tile].neighbors[3] != -1){
        player.position.x += 1;
        player.tile = tiles[player.tile].neighbors[3];
    }
    if(player.position.y > 1 && tiles[player.tile].neighbors[2] != -1){
        player.position.y -= 1;
        player.tile = tiles[player.tile].neighbors[2];
    }
    if(player.position.y < 0 && tiles[player.tile].neighbors[0] != -1){
        player.position.y += 1;
        player.tile = tiles[player.tile].neighbors[0];
    }
    
    
    //Collision
    {
        ofImage * img = &tiles[player.tile].collisionImage;
        unsigned char * pixels = img->getPixels();
        long  index = floor(player.position.x * img->getWidth()) + floor(player.position.y * img->getHeight()) * img->getWidth() ;
        if(pixels[index*3] == 0 && index*3 < img->getWidth()*img->getHeight()*3){
            ofxVec2f oldPos = oldPlayer.position;
            
            long indexX = floor((oldPlayer.position.x + player.v.x)* img->getWidth()) + floor(oldPlayer.position.y * img->getHeight()) * img->getWidth() ;
            long indexY = floor((oldPlayer.position.x)* img->getWidth()) + floor((oldPlayer.position.y + player.v.y) * img->getHeight()) * img->getWidth() ;
            
            if(pixels[indexX*3] == 0){
                player = oldPlayer;
                player.v.x *= -0.8;
            }
            
            if(pixels[indexY*3] == 0){
                player = oldPlayer;
                player.v.y *= -0.8;
                
            }
        }
        
    }
    
    
    
    
    // cout<<"Pos "<<player.position.x<<"  "<<player.position.y<<"  tile "<<player.tile<<endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDisableAlphaBlending();
    ofSetColor(0,0,0);
    ofRect(0,0,ofGetWidth(), ofGetHeight());
    glPushMatrix();
    glScaled(ofGetWidth(), ofGetHeight(), 1.0);
    
    glTranslated(0,0.3,0);//Camera movement
    
    glPushMatrix();{
        ofSetColor(255, 255, 255);
        int numRays = 2000;
        float steps = 100.0;
        ofxPoint2f _p;
        ofxVec2f _offset;
        ofImage * img;
        unsigned char * pixels;
        
        glTranslated(-player.position.x+0.5, -player.position.y+0.5, 0);
        glLineWidth(30);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(player.position.x, player.position.y);

        for(int i=0;i<numRays+1;i++){

            _offset = ofxVec2f();
            Tile * t = &tiles[player.tile];
            
            float rayIndex = TWO_PI*(float)i/numRays;
            
            for(int u=0;u<steps;u++){
                _p.x = player.position.x+sin(rayIndex)*u/steps - _offset.x;
                _p.y = player.position.y+cos(rayIndex)*u/steps - _offset.y;
                
                
                if(_p.y > 1){
                    if(t->neighbors[2] == -1){
                        break;
                    }
                    
                    t = &tiles[t->neighbors[2]];
                    _p.y -= 1;
                    _offset += ofxVec2f(0,1);
                } 
                if(_p.y < 0){
                    if(t->neighbors[0] == -1){
                        break;
                    }
                    
                    t = &tiles[t->neighbors[0]];
                    _p.y += 1;
                    _offset -= ofxVec2f(0,1);
                } 
                if(_p.x < 0){
                    if(t->neighbors[3] == -1){
                        break;
                    }
                    
                    t = &tiles[t->neighbors[3]];
                    _p.x += 1;
                    _offset -= ofxVec2f(1,0);
                }
                if(_p.x > 1){
                    if(t->neighbors[1] == -1){
                        break;
                    }
                    
                    t = &tiles[t->neighbors[1]];
                    _p.x -= 1;
                    _offset += ofxVec2f(1,0);
                }
                
                img = &t->collisionImage;
                pixels = img->getPixels();
                
                
                long index = floor(_p.x * img->getWidth()) + floor(_p.y * img->getHeight()) * img->getWidth() ;
                
                if(_p.x < 0 || _p.x > 1 || _p.y < 0 || _p.y > 1){
                    break;
                }
                if(pixels[index*3] == 0){
                    break;
                }
                
            }
            glVertex2d(_p.x+ _offset.x, _p.y+ _offset.y);
        }
        glEnd();
    } glPopMatrix();
    
    ofEnableAlphaBlending();
    glBlendFunc(GL_DST_COLOR, GL_ZERO);
    glPushMatrix();{
        //     glTranslated(player.position.x, player.position.y, 0);   
        glTranslated(0.5,0.5,0);
        // glScaled(1.2,1.2,1);        
        
        glTranslated(-player.position.x, -player.position.y,0);    
        ofSetColor(255,255,255);
        Tile * centerTile = &tiles[player.tile];
        
        
        centerTile->draw(0,0);
        if(centerTile->neighbors[0] != -1){
            tiles[centerTile->neighbors[0]].draw(0,-1);
        }
        if(centerTile->neighbors[1] != -1){
            tiles[centerTile->neighbors[1]].draw(1,0);
        }
        if(centerTile->neighbors[2] != -1){
            tiles[centerTile->neighbors[2]].draw(0,1);
        }
        if(centerTile->neighbors[3] != -1){
            tiles[centerTile->neighbors[3]].draw(-1,0);
        }
        
        
        if(centerTile->neighbors[0] != -1){
            glPushMatrix();
            glTranslated(0, -1, 0);
            Tile * subTile  = &tiles[centerTile->neighbors[0]];          
            if(subTile->neighbors[1] != -1){
                tiles[subTile->neighbors[1]].draw(1,0);
            }
            if(subTile->neighbors[3] != -1){
                tiles[subTile->neighbors[3]].draw(-1,0);
            }
            glPopMatrix();          
        }
        if(centerTile->neighbors[2] != -1){
            glPushMatrix();
            glTranslated(0, 1, 0);
            Tile * subTile  = &tiles[centerTile->neighbors[2]];  
            if(subTile->neighbors[1] != -1){
                tiles[subTile->neighbors[1]].draw(1,0);
            }
            if(subTile->neighbors[3] != -1){
                tiles[subTile->neighbors[3]].draw(-1,0);
            }
            glPopMatrix();          
        }
        
        
        
    }glPopMatrix();
    
    ofSetColor(0,0,0);
    ofCircle(0.5,0.5,0.02);
    
    
    
    
    
    glPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    float step = 0.04;
    if(key == 358){
        keyForce.x = step;
    }
    if(key == 359){
        keyForce.y = step;
        
    }
    if(key == 356){
        keyForce.x = -step;
    }
    if(key == 357){
        keyForce.y = -step;
    }
    
    if(key == 'd'){
        debug = !debug;
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == 358){
        keyForce.x *= 0.0;
    }
    if(key == 359){
        keyForce.y *= 0.0;        
    }
    if(key == 356){
        keyForce.x *= 0.0;    }
    if(key == 357){
        keyForce.y *= 0.0;    }
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}


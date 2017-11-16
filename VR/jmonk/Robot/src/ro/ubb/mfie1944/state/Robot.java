/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ro.ubb.mfie1944.state;

import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;
import com.jme3.asset.AssetManager;
import com.jme3.input.InputManager;
import com.jme3.input.KeyInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.AnalogListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Sphere;
import com.jme3.texture.Texture;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author Alex
 */
public class Robot extends AbstractAppState{

    private final Node rootNode;
    private final Node localRootNode = new Node("Robot");
    private final AssetManager assetManager;
    private final InputManager inputManager;
    
    private Node rightShoulderNode;
    private Node rightElbowNode;
    private Node leftShoulderNode;
    private Node leftElbowNode;
 
    private Node leftKneeNode;
    private Node leftJoint;
    private Node rightJoint;
    private Node rightKneeNode;
    private Node bodyNode;
    
    public Robot(SimpleApplication app){
        rootNode = app.getRootNode();
        assetManager = app.getAssetManager();
        inputManager = app.getInputManager();
    }
    
    @Override
    public void initialize(AppStateManager stateManager, Application app){
        super.initialize(stateManager, app);
        rootNode.attachChild(localRootNode);
        
        Material bodyMat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        Material headMat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        Material jointsMat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
        Material limbsMat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md"); 
        Texture bodyTexture = assetManager.loadTexture("Interface/Logo/Monkey.jpg");
        bodyMat.setTexture("ColorMap", bodyTexture);
        headMat.setColor("Color", ColorRGBA.Pink);
        jointsMat.setColor("Color", ColorRGBA.Green);
        limbsMat.setColor("Color", ColorRGBA.DarkGray);
        
        Box body = new Box(0.7f,0.9f,0.3f);
        Geometry bodyGeometry = new Geometry("Box", body);
        bodyGeometry.setMaterial(bodyMat);
        
        //-------------------------------------------------------------------------> Head
        Sphere head = new Sphere(100, 100, body.zExtent * 1.25f);
        Geometry headGeometry = new Geometry("Sphere", head);
        headGeometry.setMaterial(headMat);
        
        Node headNode = new Node("head");
        headNode.setLocalTranslation(new Vector3f(0, body.yExtent + head.radius,0));
        headNode.attachChild(headGeometry);
        
        //----------------------------------------------------------------------------
        
        
        //------------------------------------------------------------------------> Right arm
        Sphere rightShoulder = new Sphere(100, 100, body.zExtent * 0.6f);
        Geometry rightShoulderGeometry = new Geometry("Sphere", rightShoulder);
                
        Box rightArm = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry rightArmGeometry = new Geometry("Box", rightArm);
                
        Sphere rightElbow = new Sphere(100, 100, body.zExtent * 0.5f);
        Geometry rightElbowGeometry = new Geometry("Sphere", rightElbow);
                
        Box rightHand = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry rightHandGeometry = new Geometry("Box", rightHand);
                
        rightShoulderGeometry.setMaterial(jointsMat);
        rightArmGeometry.setMaterial(limbsMat);
        rightElbowGeometry.setMaterial(jointsMat);
        rightHandGeometry.setMaterial(limbsMat);
        
        Node rightHandNode = new Node("rightHand");
        rightHandNode.setLocalTranslation(new Vector3f(0,- rightHand.yExtent - rightElbow.radius/2,0));
        rightHandNode.attachChild(rightHandGeometry);
        
        rightElbowNode = new Node("rightElbow");
        rightElbowNode.setLocalTranslation(new Vector3f(0, -rightArm.yExtent, 0));
        rightElbowNode.attachChild(rightElbowGeometry);
        rightElbowNode.attachChild(rightHandNode);
               
        Node rightArmNode = new Node("rightArm");
        rightArmNode.setLocalTranslation(new Vector3f(-0.07f, -rightArm.yExtent, 0));
        rightArmNode.attachChild(rightArmGeometry);
        rightArmNode.attachChild(rightElbowNode);
        
        rightShoulderNode = new Node("rightShoulder");
        rightShoulderNode.setLocalTranslation(new Vector3f(- body.xExtent - rightArm.xExtent / 2, body.yExtent - 0.1f, 0));
        rightShoulderNode.attachChild(rightShoulderGeometry);
        rightShoulderNode.attachChild(rightArmNode);
        //-------------------------------------------------------------------------------------------------
        
        
        
        //-----------------------------------------------------------------------------------------> Left arm
        Sphere leftShoulder = new Sphere(100, 100, body.zExtent * 0.6f);
        Geometry leftShoulderGeometry = new Geometry("Sphere", leftShoulder);
        
        Box leftArm = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry leftArmGeometry = new Geometry("Box", leftArm);
        
        Sphere leftElbow = new Sphere(100, 100, body.zExtent * 0.5f);
        Geometry leftElbowGeometry = new Geometry("Sphere", leftElbow);
        
        Box leftHand = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry leftHandGeometry = new Geometry("Box", leftHand);
        
        leftShoulderGeometry.setMaterial(jointsMat);
        leftArmGeometry.setMaterial(limbsMat);
        leftElbowGeometry.setMaterial(jointsMat);
        leftHandGeometry.setMaterial(limbsMat);
        
        Node leftHandNode = new Node("leftHand");
        leftHandNode.setLocalTranslation(new Vector3f(0, -leftHand.yExtent - leftElbow.radius / 2, 0));
        leftHandNode.attachChild(leftHandGeometry);
        
        leftElbowNode = new Node("leftElbow");
        leftElbowNode.setLocalTranslation(new Vector3f(0, -leftArm.yExtent, 0));
        leftElbowNode.attachChild(leftElbowGeometry);
        leftElbowNode.attachChild(leftHandNode);
        
        Node leftArmNode = new Node("leftArm");
        leftArmNode.setLocalTranslation(new Vector3f(0.07f, -leftArm.yExtent, 0));
        leftArmNode.attachChild(leftArmGeometry);
        leftArmNode.attachChild(leftElbowNode);
        
        leftShoulderNode = new Node("leftShoulder");
        leftShoulderNode.setLocalTranslation(new Vector3f(body.xExtent + leftArm.xExtent / 2, body.yExtent - 0.1f, 0));
        leftShoulderNode.attachChild(leftShoulderGeometry);
        leftShoulderNode.attachChild(leftArmNode);
        //------------------------------------------------------------------------------------------



        //------------------------------------------------------------------------------------> Right leg
        Box rightThigh = new Box(body.xExtent / 6, body.yExtent * 0.8f, body.zExtent * 0.75f);
        Geometry rightThighGeometry = new Geometry("Box", rightThigh);
        
        Sphere rightKnee = new Sphere(100, 100, rightThigh.zExtent * 0.7f);
        Geometry rightKneeGeometry = new Geometry("Sphere", rightKnee);
        
        Box rightTibia = new Box(body.xExtent / 6, body.yExtent * 0.9f, body.zExtent * 0.75f);
        Geometry rightTibiaGeometry = new Geometry("Box", rightTibia);
        
        rightThighGeometry.setMaterial(limbsMat);
        rightKneeGeometry.setMaterial(jointsMat);
        rightTibiaGeometry.setMaterial(limbsMat);
        
        Node rightTibiaNode = new Node("rightTibia");
        rightTibiaNode.setLocalTranslation(new Vector3f(0, -rightTibia.yExtent, 0));
        rightTibiaNode.attachChild(rightTibiaGeometry);
        
        rightKneeNode = new Node("rightKnee");
        rightKneeNode.setLocalTranslation(new Vector3f(0, -rightThigh.yExtent - rightKnee.radius / 2, 0));
        rightKneeNode.attachChild(rightKneeGeometry);
        rightKneeNode.attachChild(rightTibiaNode);
        
        Node rightThighNode = new Node("rightThigh");
        rightThighNode.setLocalTranslation(new Vector3f(0, -rightThigh.yExtent, 0));
        rightThighNode.attachChild(rightKneeNode);
        rightThighNode.attachChild(rightThighGeometry);
        
        rightJoint = new Node("rightJoint");
        rightJoint.setLocalTranslation(new Vector3f(-body.xExtent + rightThigh.xExtent * 3, -body.yExtent, 0));
        rightJoint.attachChild(rightThighNode);
        //------------------------------------------------------------------------------------------
        
        
        
        //----------------------------------------------------------------------------------> Left leg 
        Box leftThigh = new Box(body.xExtent / 6, body.yExtent * 0.8f, body.zExtent * 0.75f);
        Geometry leftThighGeometry = new Geometry("Box", leftThigh);
        
        Sphere leftKnee = new Sphere(100, 100, leftThigh.zExtent * 0.7f);
        Geometry leftKneeGeometry = new Geometry("Sphere", leftKnee);
        
        Box leftTibia = new Box(body.xExtent / 6, body.yExtent * 0.9f, body.zExtent * 0.75f);
        Geometry leftTibiaGeometry = new Geometry("Box", leftTibia);
        
        leftThighGeometry.setMaterial(limbsMat); 
        leftKneeGeometry.setMaterial(jointsMat);
        leftTibiaGeometry.setMaterial(limbsMat);
       
        Node leftTibiaNode = new Node("leftTibia");
        leftTibiaNode.setLocalTranslation(new Vector3f(0, -leftTibia.yExtent, 0));
        leftTibiaNode.attachChild(leftTibiaGeometry);
        
        leftKneeNode = new Node("leftKnee");
        leftKneeNode.setLocalTranslation(new Vector3f(0, -leftThigh.yExtent - leftKnee.radius / 2, 0));
        leftKneeNode.attachChild(leftKneeGeometry);
        leftKneeNode.attachChild(leftTibiaNode);
        
        Node leftThighNode = new Node("leftThigh");
        leftThighNode.setLocalTranslation(new Vector3f(0, -leftThigh.yExtent, 0));
        leftThighNode.attachChild(leftKneeNode);
        leftThighNode.attachChild(leftThighGeometry);
        
        leftJoint = new Node("leftJoint");
        leftJoint.setLocalTranslation(new Vector3f(body.xExtent - leftThigh.xExtent * 3, -body.yExtent, 0));
        leftJoint.attachChild(leftThighNode);
        
        //----------------------------------------------------------------------> Body
        bodyNode = new Node("body");
        bodyNode.attachChild(bodyGeometry);
        bodyNode.attachChild(headNode);
        bodyNode.attachChild(leftShoulderNode);
        bodyNode.attachChild(rightShoulderNode);
        bodyNode.attachChild(leftJoint);
        bodyNode.attachChild(rightJoint);
        
        localRootNode.attachChild(bodyNode);
        initKeys();
    }
    
    private void initKeys(){
        inputManager.addMapping("Toggle", new KeyTrigger(KeyInput.KEY_T));
        inputManager.addMapping("LeftS", new KeyTrigger(KeyInput.KEY_L));
        inputManager.addMapping("RightS", new KeyTrigger(KeyInput.KEY_R));
        inputManager.addMapping("LeftSD", new KeyTrigger(KeyInput.KEY_K));
        inputManager.addMapping("RightSD", new KeyTrigger(KeyInput.KEY_E));
        inputManager.addMapping("LeftE", new KeyTrigger(KeyInput.KEY_1));
        inputManager.addMapping("RightE", new KeyTrigger(KeyInput.KEY_9));
        inputManager.addMapping("LeftED", new KeyTrigger(KeyInput.KEY_2));
        inputManager.addMapping("RightED", new KeyTrigger(KeyInput.KEY_0));
        inputManager.addMapping("Legs", new KeyTrigger(KeyInput.KEY_G));
        inputManager.addMapping("LegsB", new KeyTrigger(KeyInput.KEY_H));
        inputManager.addMapping("Jump", new KeyTrigger(KeyInput.KEY_T));
        inputManager.addMapping("JumpB", new KeyTrigger(KeyInput.KEY_Y));
        inputManager.addMapping("Body", new KeyTrigger(KeyInput.KEY_B));
        inputManager.addListener(analogListener, "LeftS", "RightS", "LeftE", "RightE",
                "LeftSD", "RightSD", "LeftED", "RightED", "Legs", "LegsB", "Body", "Jump", "JumpB");
    }
    
    private final AnalogListener analogListener = new AnalogListener(){
        @Override
        public void onAnalog(String name, float value, float tpf){
            if (name.equals("RightS")){
                rightShoulderNode.rotate(0, 0, -3f * tpf);
            }
            else if (name.equals("RightSD")){
                rightShoulderNode.rotate(0, 0, 3f * tpf);
            }
            
            if (name.equals("LeftS")){
                leftShoulderNode.rotate(0, 0, 3f * tpf);
            }
            else if (name.equals("LeftSD")){
                leftShoulderNode.rotate(0, 0, -3f * tpf);
            }
            
            if (name.equals("RightE")){
                rightElbowNode.rotate(0, 0, -3f * tpf);
            }
            else if (name.equals("RightED")){
                rightElbowNode.rotate(0, 0, 3f * tpf);
            }
            
            if (name.equals("LeftE")){
                leftElbowNode.rotate(0, 0, 3f * tpf);
            }
            else if (name.equals("LeftED")){
                leftElbowNode.rotate(0, 0, -3f * tpf);
            }
            
            if (name.equals("Legs")){
                leftJoint.rotate(0, 0, 3f * tpf);
                leftKneeNode.rotate(0, 0, -3f * tpf);
                rightJoint.rotate(0, 0, -3f * tpf);
                rightKneeNode.rotate(0, 0, 3f * tpf);
            }
            else if(name.equals("LegsB")){
                leftJoint.rotate(0 ,0, -3f * tpf);
                leftKneeNode.rotate(0, 0, 3f * tpf);
                rightJoint.rotate(0, 0, 3f * tpf);
                rightKneeNode.rotate(0, 0, -3f * tpf);
            }
            
            if (name.equals("Jump")){
                leftJoint.rotate(-3f * tpf, 0, 0);
                leftKneeNode.rotate(3f * tpf, 0, 0);
                rightJoint.rotate(-3f * tpf, 0 ,0);
                rightKneeNode.rotate(3f * tpf, 0 ,0);
            }
            else if (name.equals("JumpB")){
                leftJoint.rotate(3f * tpf, 0, 0);
                leftKneeNode.rotate(-3f * tpf, 0, 0);
                rightJoint.rotate(3f * tpf, 0 ,0);
                rightKneeNode.rotate(-3f * tpf, 0 ,0);
            }
            
            if (name.equals("Body")){
                bodyNode.rotate(0, 3f * tpf, 0);
            }
        }
    };
    
    @Override
    public void cleanup(){
        rootNode.detachChild(localRootNode);
        super.cleanup();
    }
}

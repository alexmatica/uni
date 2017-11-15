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
        
        Map<String, Material> materials = new HashMap<>(); 
        materials.put("body", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); //BODY
        materials.put("head", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); //HEAD
        materials.put("shoulder", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); // SHOULDER
        materials.put("upperArm", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); //UPPER ARMS
        materials.put("elbow", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md"));    //ELBOW
        materials.put("lowerArm", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); //LOWER ARMS
        materials.put("upperLeg", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); //UPPER LEGS
        materials.put("lowerLeg", new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md")); //LOWER LEGS
        
        Texture bodyTexture = assetManager.loadTexture("Interface/Logo/Monkey.jpg");
        materials.get("body").setTexture("ColorMap", bodyTexture);
        materials.get("head").setColor("Color", ColorRGBA.Pink);
        materials.get("shoulder").setColor("Color", ColorRGBA.Green);
        materials.get("upperArm").setColor("Color", ColorRGBA.DarkGray);
        materials.get("elbow").setColor("Color", ColorRGBA.Green);
        materials.get("lowerArm").setColor("Color", ColorRGBA.DarkGray);
        materials.get("upperLeg").setColor("Color", ColorRGBA.DarkGray);
        materials.get("lowerLeg").setColor("Color", ColorRGBA.DarkGray);
        
        
        Box body = new Box(0.7f,0.9f,0.3f);
        Geometry bodyGeometry = new Geometry("Box", body);
        bodyGeometry.setLocalTranslation(new Vector3f(0,0,0));
        bodyGeometry.setMaterial(materials.get("body"));
        
        //-------------------------------------------------------------------------> Head
        Sphere head = new Sphere(100, 100, body.zExtent * 1.25f);
        Geometry headGeometry = new Geometry("Sphere", head);
        headGeometry.setLocalTranslation(new Vector3f(0, 0, 0));
        headGeometry.setMaterial(materials.get("head"));
        
        Node headNode = new Node("head");
        headNode.setLocalTranslation(new Vector3f(0,head.radius,0));
        headNode.attachChild(headGeometry);
        
        Node neckNode = new Node("neck");
        neckNode.setLocalTranslation(new Vector3f(0, body.yExtent, 0));
        neckNode.attachChild(headNode);
        //----------------------------------------------------------------------------
        
        
        //------------------------------------------------------------------------> Right arm
        Sphere rightShoulder = new Sphere(100, 100, body.zExtent * 0.6f);
        Geometry rightShoulderGeometry = new Geometry("Sphere", rightShoulder);
                
        Box rightUpperArm = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry rightUpperArmGeometry = new Geometry("Box", rightUpperArm);
                
        Sphere rightElbow = new Sphere(100, 100, body.zExtent * 0.5f);
        Geometry rightElbowGeometry = new Geometry("Sphere", rightElbow);
                
        Box rightLowerArm = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry rightLowerArmGeometry = new Geometry("Box", rightLowerArm);
                
        rightShoulderGeometry.setMaterial(materials.get("shoulder"));
        rightUpperArmGeometry.setMaterial(materials.get("upperArm"));
        rightElbowGeometry.setMaterial(materials.get("elbow"));
        rightLowerArmGeometry.setMaterial(materials.get("lowerArm"));
        
        Node rightLowerArmNode = new Node("rightLowerArm");
        rightLowerArmNode.setLocalTranslation(new Vector3f(0,- rightLowerArm.yExtent - rightElbow.radius/2,0));
        rightLowerArmNode.attachChild(rightLowerArmGeometry);
        
        rightElbowNode = new Node("rightElbow");
        rightElbowNode.setLocalTranslation(new Vector3f(0, -rightUpperArm.yExtent, 0));
        rightElbowNode.attachChild(rightElbowGeometry);
        rightElbowNode.attachChild(rightLowerArmNode);
               
        Node rightUpperArmNode = new Node("rightUpperArm");
        rightUpperArmNode.setLocalTranslation(new Vector3f(-0.07f, -rightUpperArm.yExtent, 0));
        rightUpperArmNode.attachChild(rightUpperArmGeometry);
        rightUpperArmNode.attachChild(rightElbowNode);
        
        rightShoulderNode = new Node("rightShoulder");
        rightShoulderNode.setLocalTranslation(new Vector3f(- body.xExtent - rightUpperArm.xExtent / 2, body.yExtent - 0.1f, 0));
        rightShoulderNode.attachChild(rightShoulderGeometry);
        rightShoulderNode.attachChild(rightUpperArmNode);
        //-------------------------------------------------------------------------------------------------
        
        
        
        //-----------------------------------------------------------------------------------------> Left arm
        Sphere leftShoulder = new Sphere(100, 100, body.zExtent * 0.6f);
        Geometry leftShoulderGeometry = new Geometry("Sphere", leftShoulder);
        
        Box leftUpperArm = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry leftUpperArmGeometry = new Geometry("Box", leftUpperArm);
        
        Sphere leftElbow = new Sphere(100, 100, body.zExtent * 0.5f);
        Geometry leftElbowGeometry = new Geometry("Sphere", leftElbow);
        
        Box leftLowerArm = new Box(body.xExtent / 6, body.yExtent * 0.6f, body.zExtent * 0.75f);
        Geometry leftLowerArmGeometry = new Geometry("Box", leftLowerArm);
        
        leftShoulderGeometry.setMaterial(materials.get("shoulder"));
        leftUpperArmGeometry.setMaterial(materials.get("upperArm"));
        leftElbowGeometry.setMaterial(materials.get("elbow"));
        leftLowerArmGeometry.setMaterial(materials.get("lowerArm"));
        
        Node leftLowerArmNode = new Node("leftLowerArm");
        leftLowerArmNode.setLocalTranslation(new Vector3f(0, -leftLowerArm.yExtent - leftElbow.radius / 2, 0));
        leftLowerArmNode.attachChild(leftLowerArmGeometry);
        
        leftElbowNode = new Node("leftElbow");
        leftElbowNode.setLocalTranslation(new Vector3f(0, -leftUpperArm.yExtent, 0));
        leftElbowNode.attachChild(leftElbowGeometry);
        leftElbowNode.attachChild(leftLowerArmNode);
        
        Node leftUpperArmNode = new Node("leftUpperArm");
        leftUpperArmNode.setLocalTranslation(new Vector3f(0.07f, -leftUpperArm.yExtent, 0));
        leftUpperArmNode.attachChild(leftUpperArmGeometry);
        leftUpperArmNode.attachChild(leftElbowNode);
        
        leftShoulderNode = new Node("leftShoulder");
        leftShoulderNode.setLocalTranslation(new Vector3f(body.xExtent + leftUpperArm.xExtent / 2, body.yExtent - 0.1f, 0));
        leftShoulderNode.attachChild(leftShoulderGeometry);
        leftShoulderNode.attachChild(leftUpperArmNode);
        //------------------------------------------------------------------------------------------



        //------------------------------------------------------------------------------------> Right leg
        Box rightUpperLeg = new Box(body.xExtent / 6, body.yExtent * 0.8f, body.zExtent * 0.75f);
        Geometry rightUpperLegGeometry = new Geometry("Box", rightUpperLeg);
        
        Sphere rightKnee = new Sphere(100, 100, rightUpperLeg.zExtent * 0.7f);
        Geometry rightKneeGeometry = new Geometry("Sphere", rightKnee);
        
        Box rightLowerLeg = new Box(body.xExtent / 6, body.yExtent * 0.9f, body.zExtent * 0.75f);
        Geometry rightLowerLegGeometry = new Geometry("Box", rightLowerLeg);
        
        rightUpperLegGeometry.setMaterial(materials.get("upperLeg"));
        rightKneeGeometry.setMaterial(materials.get("elbow"));
        rightLowerLegGeometry.setMaterial(materials.get("lowerLeg"));
        
        Node rightLowerLegNode = new Node("rightLowerLeg");
        rightLowerLegNode.setLocalTranslation(new Vector3f(0, -rightLowerLeg.yExtent, 0));
        rightLowerLegNode.attachChild(rightLowerLegGeometry);
        
        rightKneeNode = new Node("rightKnee");
        rightKneeNode.setLocalTranslation(new Vector3f(0, -rightUpperLeg.yExtent - rightKnee.radius / 2, 0));
        rightKneeNode.attachChild(rightKneeGeometry);
        rightKneeNode.attachChild(rightLowerLegNode);
        
        Node rightUpperLegNode = new Node("rightUpperLeg");
        rightUpperLegNode.setLocalTranslation(new Vector3f(0, -rightUpperLeg.yExtent, 0));
        rightUpperLegNode.attachChild(rightKneeNode);
        rightUpperLegNode.attachChild(rightUpperLegGeometry);
        
        rightJoint = new Node("rightJoint");
        rightJoint.setLocalTranslation(new Vector3f(-body.xExtent + rightUpperLeg.xExtent * 3, -body.yExtent, 0));
        rightJoint.attachChild(rightUpperLegNode);
        //------------------------------------------------------------------------------------------
        
        
        
        //----------------------------------------------------------------------------------> Left leg 
        Box leftUpperLeg = new Box(body.xExtent / 6, body.yExtent * 0.8f, body.zExtent * 0.75f);
        Geometry leftUpperLegGeometry = new Geometry("Box", leftUpperLeg);
        
        Sphere leftKnee = new Sphere(100, 100, leftUpperLeg.zExtent * 0.7f);
        Geometry leftKneeGeometry = new Geometry("Sphere", leftKnee);
        
        Box leftLowerLeg = new Box(body.xExtent / 6, body.yExtent * 0.9f, body.zExtent * 0.75f);
        Geometry leftLowerLegGeometry = new Geometry("Box", leftLowerLeg);
        
        leftUpperLegGeometry.setMaterial(materials.get("upperLeg")); 
        leftKneeGeometry.setMaterial(materials.get("elbow"));
        leftLowerLegGeometry.setMaterial(materials.get("lowerLeg"));
       
        Node leftLowerLegNode = new Node("leftLowerLeg");
        leftLowerLegNode.setLocalTranslation(new Vector3f(0, -leftLowerLeg.yExtent, 0));
        leftLowerLegNode.attachChild(leftLowerLegGeometry);
        
        leftKneeNode = new Node("leftKnee");
        leftKneeNode.setLocalTranslation(new Vector3f(0, -leftUpperLeg.yExtent - leftKnee.radius / 2, 0));
        leftKneeNode.attachChild(leftKneeGeometry);
        leftKneeNode.attachChild(leftLowerLegNode);
        
        Node leftUpperLegNode = new Node("leftUpperLeg");
        leftUpperLegNode.setLocalTranslation(new Vector3f(0, -leftUpperLeg.yExtent, 0));
        leftUpperLegNode.attachChild(leftKneeNode);
        leftUpperLegNode.attachChild(leftUpperLegGeometry);
        
        leftJoint = new Node("leftJoint");
        leftJoint.setLocalTranslation(new Vector3f(body.xExtent - leftUpperLeg.xExtent * 3, -body.yExtent, 0));
        leftJoint.attachChild(leftUpperLegNode);
        
        //----------------------------------------------------------------------> Body
        bodyNode = new Node("body");
        bodyNode.attachChild(bodyGeometry);
        bodyNode.attachChild(neckNode);
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
        inputManager.addMapping("Body", new KeyTrigger(KeyInput.KEY_B));
        inputManager.addListener(analogListener, "LeftS", "RightS", "LeftE", "RightE",
                "LeftSD", "RightSD", "LeftED", "RightED", "Legs", "LegsB", "Body");
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

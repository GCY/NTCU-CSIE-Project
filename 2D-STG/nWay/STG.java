/*	JAVA 2D-STG 
 *
 *	2011//12//29
 *	
 *	by GCY
 *
 */

import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;
import java.lang.*;
import java.applet.Applet;
import java.util.concurrent.locks.*;

import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;

abstract class BaseObject
{
   public double x,y,speed;

   public boolean Remove(){return false;}

   abstract public void Draw(Graphics g);
   abstract public void Update();
}

abstract class Plane extends BaseObject
{
   public double hp;

   public void Update(){}
   public void Update(double move_x,double move_y){}
   
   public int Fire(List Bullet){return 0;}
   public int Fire(List Bullet,double tx,double ty){return 0;}
}

abstract class Bullet extends BaseObject
{
   public double power;

   public boolean HitTest(double tx,double ty)
   {
      if(tx < this.x + 5 && 
	    tx + 20 > this.x + 5 &&
	    ty < this.y + 5 &&
	    ty + 20 > this.y + 5){
	 return true;
      }
      return false;
   }
}

class GeneralBullet extends Bullet
{
   GeneralBullet(double x,double y,double power,double speed)
   {
      this.x = x;
      this.y = y;
      this.power = power;
      this.speed = speed;
   }

   public void Draw(Graphics g)
   {
      Graphics2D g2D = (Graphics2D)g;
      g2D.setPaint(Color.yellow);
      
      Ellipse2D.Double bullet = new Ellipse2D.Double(x,y,10,10);
      g2D.fill(bullet);
      g2D.draw(bullet);
   }

   public void Update()
   {
      double move_x = 0;
      double move_y = -1;
      this.x += move_x * speed;
      this.y += move_y * speed;
   }

   public boolean Remove()
   {
      if(this.y < 0){return true;}
      return false;
   }
}

class nWayBullet extends Bullet
{
   private double angle;
   Color color = Color.red;
   nWayBullet(double x,double y,double angle,double power,double speed)
   {
      this.x = x;
      this.y = y;
      this.angle = angle;
      this.power = power;
      this.speed = speed;
   }

   nWayBullet(double x,double y,double angle,double power,double speed,Color color)
   {
      this.x = x;
      this.y = y;
      this.angle = angle;
      this.power = power;
      this.speed = speed;
      this.color = color;
   }

   public void Draw(Graphics g)
   {
      Graphics2D g2D = (Graphics2D)g;
      g2D.setPaint(color);
      
      Ellipse2D.Double bullet = new Ellipse2D.Double(x + 5,y + 5,10,10);
      g2D.fill(bullet);
      g2D.draw(bullet);
   }

   public void Update()
   {
      this.x += Math.cos(Math.toRadians(angle)) * speed;
      this.y += Math.sin(Math.toRadians(angle)) * speed;
   }

   public boolean Remove()
   {
      if(this.x < 0 ||
	    this.x > 440 ||
	    this.y < 0 ||
	    this.y > 480){
	 return true;
      }
      return false;
   }
}

class MyPlane extends Plane
{
   private long fDelay = 0;
   private final long dTime = 80000000;

   MyPlane(double x,double y,double hp,double speed)
   {
      this.x = x;
      this.y = y;
      this.hp = hp;
      this.speed = speed;
   }

   public void Draw(Graphics g)
   {
      g.setColor(Color.blue);
      g.fillRect((int)x,(int)y,20,20);

      g.setColor(Color.red);
      for(int i = 1;i <= hp;i++){
	 g.fillRect(490 + i,450,1,3);
      }

   }

   public void Update(double move_x,double move_y)
   {
      this.x += move_x * speed;
      this.y += move_y * speed;

      if(this.x > 440){
	 this.x = 440;
      }
      else if(this.x < 0){
	 this.x = 0;
      }
      else if(this.y > 460){
	 this.y = 460;
      }
      else if(this.y < 0){
	 this.y = 0;
      }
   }

   public int Fire(List Bullet)
   {
      double speed = 20;
      double atk = 20;
      if((System.nanoTime() - fDelay) > dTime){
	 fDelay = System.nanoTime();
	 Bullet.add(new GeneralBullet(this.x + 5,this.y + 5,atk,speed));
	 Bullet.add(new GeneralBullet(this.x - 5,this.y - 5,atk,speed));
	 Bullet.add(new GeneralBullet(this.x + 15,this.y - 5,atk,speed));
	 return 3;
      }
      return 0;
   }

   public int Special(List Bullet)
   {
      int num = 5;
      double angle = 15;
      double start = 240;
      double speed = 18;
      double atk = 15;
      if((System.nanoTime() - fDelay) > dTime){
	 fDelay = System.nanoTime();
	 for(int i = 0;i < num;i++){
	    Bullet.add(new nWayBullet(this.x,this.y,(start + (i * angle)),atk,speed,Color.yellow));
	 }
	 return num;
      }
      return 0;
   }
}

class GeneralEnemyPlane extends Plane
{
   private long fDelay = 0;
   private final long dTime = 1000000000;

   private int path;

   GeneralEnemyPlane(double hp,double speed,int path)
   {
      this.hp = hp;
      this.speed = speed;
      this.path = path;

      if(path == 1){
	 this.x = 30;
	 this.y = -20;
      }
      else if(path == 2){
	 this.x = 130;
	 this.y = -40;
      }
      else if(path == 3){
	 this.x = 170;
	 this.y = -40;
      }
      else if(path == 4){
	 this.x = 230;
	 this.y = -20;

      }
      else if(path == 5){
	 this.x = 330;
	 this.y = -20;

      }
   }

   public void Draw(Graphics g)
   {
      g.setColor(Color.green);
      g.fillRect((int)x,(int)y,20,20);
   }

   public void Update(double move_x,double move_y)
   {
      this.x += move_x * speed;
      this.y += move_y * speed;
   }

   public boolean Remove()
   {
      if(this.y > 640){return true;}
      return false;
   }

   public int Fire(List Bullet)
   {
      int num = (int)(Math.random() * 36 + 12);
      double angle = 360 / num;
      double speed = 2.8;
      double atk = 8;
      if((System.nanoTime() - fDelay) > dTime){
	 fDelay = System.nanoTime();
	 for(int i = 0;i < num;i++){
	    Bullet.add(new nWayBullet(this.x,this.y,i * angle,atk,speed));
	 }
	 return num;
      }
      return 0;
   }
}

public class STG 
	extends Applet
	implements KeyListener, Runnable
{
   final long sec = 1000000000;
   long now = System.nanoTime();
   long last = 0;
   long lastTemp = 0;
   int fps = 60,lastFps = 0;

   int destroy = 0;
   int lose = 0;
   int runtime = 0;
   int totalBullet = 0;

   boolean left,right,up,down,fire,shift;

   Image frameBuffer;

   ReentrantLock lock = new ReentrantLock();

   MyPlane my = new MyPlane(240,300,150,3);
   List<Bullet> gBullet = new LinkedList<Bullet>();
   List<Bullet> sBullet = new LinkedList<Bullet>();

   final long eDelay = 1000000000;
   long lTime = 0;
   List<Plane> enemy = new LinkedList<Plane>();
   List<Bullet> nBullet = new LinkedList<Bullet>();

   ListIterator<Bullet> BulletIter;
   ListIterator<Plane> PlaneIter; 

   public STG()
   {
      new Thread(this).start();
      addKeyListener(this);
   }

   public void paint(Graphics g)
   {
      lock.lock();

      g.fillRect(0, 0, 640, 480);
      g.setColor(new Color(212,145,38));
      g.fillRect(460,0,180,480);


      my.Draw(g);

      BulletIter = gBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 b.Draw(g);
      }

      PlaneIter = enemy.listIterator();
      while(PlaneIter.hasNext()){
	 Plane b = PlaneIter.next();
	 b.Draw(g);
      }

      BulletIter = nBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 b.Draw(g);
      }

      BulletIter = sBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 b.Draw(g);
      }

      g.setFont(new Font("Arial Narrow",Font.PLAIN,12));
      g.setColor(new Color(200,50,200));
      g.drawString("HP：",460,456);
      g.setFont(new Font("Arial Narrow",Font.PLAIN,24));
      g.setColor(new Color(200,50,200));
      g.drawString("FPS : " + String.valueOf(lastFps),500,20);
      g.drawString("Destroy : " + String.valueOf(destroy),480,80);
      g.drawString("Lose : " + String.valueOf(lose),500,140);
      g.drawString("Run : " + String.valueOf(runtime) + "/sec",470,200);
      g.drawString("Bullet : " + String.valueOf(totalBullet),480,260);
      
      lock.unlock();
   }

   public void updateObjects()
   {
      lock.lock();

      /**********Self**********/

      if(up){my.Update(0,-1);}
      if(down){my.Update(0,1);}
      if(left){my.Update(-1,0);}
      if(right){my.Update(1,0);}

      if(fire && shift){
	 totalBullet += my.Special(sBullet);
      }
      else if(fire){
	 totalBullet += my.Fire(gBullet);
      }

      if((System.nanoTime() - lTime) > eDelay){
	 lTime = System.nanoTime();
	 enemy.add(new GeneralEnemyPlane(150,3,(int)(Math.random() * 5 + 1)));
      }

      BulletIter = gBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 b.Update();
	 if(b.Remove()){
	    totalBullet--;
	    BulletIter.remove();
	 }
      }

      BulletIter = gBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 ListIterator<Plane> ene = enemy.listIterator();
	 while(ene.hasNext()){
	    Plane f = ene.next();
	    if(b.HitTest(f.x,f.y)){
	       f.hp -= b.power;
	       totalBullet--;
	       BulletIter.remove();
	       if(f.hp < 0){
		  destroy++;
	       }
	    }
	 }
      }

      BulletIter = sBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 b.Update();
	 if(b.Remove()){
	    totalBullet--;
	    BulletIter.remove();
	 }
      }

      BulletIter = sBullet.listIterator();
      while(BulletIter.hasNext()){
	 Bullet b = BulletIter.next();
	 ListIterator<Plane> ene = enemy.listIterator();
	 while(ene.hasNext()){
	    Plane f = ene.next();
	    if(b.HitTest(f.x,f.y)){
	       f.hp -= b.power;
	       totalBullet--;
	       BulletIter.remove();
	       if(f.hp < 0){
		  destroy++;
	       }
	    }
	 }
      }

      /**********Enemy**********/

      PlaneIter = enemy.listIterator();
      while(PlaneIter.hasNext()){
	 Plane b = PlaneIter.next();
	 b.Update(0,0.5);
	 if(b.Remove() || b.hp < 0){PlaneIter.remove();}
      }

      PlaneIter = enemy.listIterator();
      while(PlaneIter.hasNext()){
	 Plane p = PlaneIter.next();
	 totalBullet += p.Fire(nBullet);
      }

      BulletIter = nBullet.listIterator();
	 while(BulletIter.hasNext()){
	    Bullet b = BulletIter.next();
	    b.Update();
	    if(b.Remove()){
	       totalBullet--;
	       BulletIter.remove();
	    }
	 }

      BulletIter = nBullet.listIterator();
	 while(BulletIter.hasNext()){
	    Bullet b = BulletIter.next();
	    if(b.HitTest(my.x,my.y)){
	       my.hp -= b.power;
	       totalBullet--;
	       BulletIter.remove();
	       if(my.hp < 0){
		  my.hp = 150;
		  lose++;
	       }
	    }
	 }

      lock.unlock();
   }

   public void update(Graphics g)
   {
      Graphics temp;
      Dimension sz = getSize();
      
      if (frameBuffer == null ||(!(frameBuffer.getWidth(this) == sz.width &&
		  frameBuffer.getHeight(this) == sz.height))){
	 frameBuffer = createImage(sz.width, sz.height);
      }

      temp = frameBuffer.getGraphics();

      paint(temp);

      g.drawImage(frameBuffer, 0, 0, this);
   }

   void handleKey(KeyEvent eve,boolean pressed)
   {
      int key = eve.getKeyCode();

      switch(key)
      {
	 case KeyEvent.VK_LEFT:
	       left = pressed;break;
	 case KeyEvent.VK_RIGHT:
	       right = pressed;break;
	 case KeyEvent.VK_UP:
	       up = pressed;break;
	 case KeyEvent.VK_DOWN:
	       down = pressed;break;
	 case KeyEvent.VK_SPACE:
	       fire = pressed;break;
	 case KeyEvent.VK_SHIFT:
	       shift = pressed;break;
      }
   }

   public void keyPressed(KeyEvent eve)
   {
      handleKey(eve,true);
   }

   public void keyTyped(KeyEvent eve){}

   public void keyReleased(KeyEvent eve)
   {
      handleKey(eve,false);
   }

   public void run()
   {
      while(true){
	 now = System.nanoTime();
	 if(now - lastTemp >= sec){
	    lastFps = fps;
	    fps = 0;
	    lastTemp = now;
	    runtime++;
	 }
	 if(now - last >= sec / 60){
	    fps++;
	    last = now;
	    updateObjects();
	    repaint();
	 }
      }
   }
}

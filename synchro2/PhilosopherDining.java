public class PhilosopherDining {

   public static void main(String[] args) {
      final Table table = buildTable(new String[] { "Plato", "Aristole", "Socrates", "Locke", "Hobbes" });

      for(final Philosopher p : table.getPhilosophers()) {
         p.start();
      }

      new Thread() {
         public void run() {
            while(true) {
               table.print();
               try {
                  Thread.sleep(1000);
               } catch (InterruptedException ie) {}
            }
         }
      }.start();
   }

   private static Table buildTable(final String[] names) {
      final Philosopher[] phi = new Philosopher[names.length];
      final Fork[] fork = new Fork[names.length];

      for(int i = 0; i < fork.length; i++) {
         fork[i] = new Fork();
      }

      for(int i = 0; i < phi.length; i++) {
         phi[i] = new Philosopher(names[i], fork[i], fork[(i + 1) % fork.length]);
      }

      return new Table(phi, fork);
   }

   private static class Table {

      private final Philosopher[] phi;
      private final Fork[] fork;

      Table(final Philosopher[] phi, final Fork[] fork) {
         this.phi = phi;
         this.fork = fork;
      }

      Philosopher[] getPhilosophers() {
         return phi;
      }

      Fork[] getForks() {
         return fork;
      }

      void print() {
         System.out.println();

         for(final Philosopher p : phi) {
            System.out.println(p.name() + " is " + p.getStatus() + ". [eaten " + p.getEatCount() + " times]");
         }

         final StringBuilder builder = new StringBuilder();
         builder.append("Fork status [");
         for(int i = 0; i < fork.length; i++) {
            if(i != 0)
               builder.append(" || ");
            builder.append((i + 1));
            builder.append(": ");
            builder.append(fork[i].isLocked());
         }
         builder.append("]");

         System.out.println(builder.toString());
      }

   }

   private static class Philosopher extends Thread {

      private final String name;
      private final Fork fork1, fork2;

      private String status;
      private int eatCount;

      Philosopher(final String name, final Fork fork1, final Fork fork2) {
         this.name = name;
         this.fork1 = fork1;
         this.fork2 = fork2;
      }

      String name() {
         return name;
      }

      String getStatus() {
         return status;
      }

      int getEatCount() {
         return eatCount;
      }

      @Override
      public void run() {
         while(true) {
            think();
            getForks();
            eat();
            putForks();
         }
      }

      private void getForks() {
         status = "attempting to acquire forks";
         while(true) {
            if(fork1.acquireLock())
               if(fork2.acquireLock())
                  break;
               else
                  fork1.releaseLock();
            try {
               Thread.sleep((long) (Math.random() * 1000));
            } catch (InterruptedException ie) {
            }
         }

      }

      private void putForks() {
         status = "releasing forks";
         fork1.releaseLock();
         fork2.releaseLock();
      }

      private void think() {
         status = "thinking";
         try {
            Thread.sleep((long) (Math.random() * 19000L) + 1000L);
         } catch (InterruptedException ie) {
         }
      }

      private void eat() {
         status = "eating";
         eatCount++;
         try {
            Thread.sleep((long) (Math.random() * 7000L) + 2000L);
         } catch (InterruptedException ie) {
         }
      }

   }

   private static class Fork {

      private boolean locked = false;

      boolean acquireLock() {
         synchronized(this) {
            if(!locked) {
               locked = true;
               return true;
            }
            return false;
         }
      }

      void releaseLock() {
         synchronized(this) {
            locked = false;
         }
      }

      boolean isLocked() {
         return locked;
      }

   }

}

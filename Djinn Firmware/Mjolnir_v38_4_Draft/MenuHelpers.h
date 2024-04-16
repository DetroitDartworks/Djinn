// Menu object types
// Just chucked into a header because I don't want to have to have this at the top of the INO file :p

// This does range testing on the Config screen Values, Oled display

class MenuItem
{
  public:
  int CurrentValue = 0;
  char Title[8] = "\0";
  char Output[8] = "\0";

  virtual void Increase(  ) = 0;
  virtual void Decrease(  ) = 0; 
  virtual void PrepareOutput( ) = 0;
};

// ------------------------------------------------------------------------------
class GameTimeMenuItem : public MenuItem       // Game Timer Menu Item 1
{
  private:
    
    
  public:

  void Increase()
  {
    CurrentValue += 1;
    if( CurrentValue > 120 ) CurrentValue = 120;
  }
  void Decrease()
  {
    CurrentValue -= 1;
    if( CurrentValue < 0 ) CurrentValue = 0;   
  }
  void PrepareOutput()
  {
    sprintf( Output, "%4d", CurrentValue ); 
  }
};

// ------------------------------------------------------------------------------
class MotorMenuItem : public MenuItem       // Power Output Menu Item 2 & 3
{
  private:
    
    
  public:

  void Increase()
  {
    CurrentValue += 5;
    if( CurrentValue > 100 ) CurrentValue = 100;
  }
  void Decrease()
  {
    CurrentValue -= 5;
    if( CurrentValue < 30 ) CurrentValue = 30;   
  }
  void PrepareOutput()
  {
    sprintf( Output, "%3d%%", CurrentValue ); 
  }
};

// ----------------------------------------------------------------
class ROFMenuItem : public MenuItem         // ROF Menu Item 4 & 5 
{
  private:
    
    
  public:

  void Increase()
  {
    if( CurrentValue < 150 )
      CurrentValue += 10;
  }
  void Decrease()
  {
    if( CurrentValue > 0 )
      CurrentValue -= 10 ;
      
  }
  void PrepareOutput()
  {
    if( CurrentValue == 0 )
    {
      strcpy( Output, " MAX" );
    }
    else
    {
      sprintf( Output, "%4d", CurrentValue ); 
    }
  }
};

// --------------------------------------------------------------------------------
class BurstSizeMenuItem : public MenuItem     // Burst Size Menu Item 6
{
  private:
    
    
  public:

  void Increase()
  {
    if( CurrentValue < 49 )
      CurrentValue ++;
  }
  void Decrease()
  {
    if( CurrentValue > 1 )
      CurrentValue --;
  }
  void PrepareOutput()
  {
    sprintf( Output, "%4d", CurrentValue ); 
  }
};

// -------------------------------------------------------------------------
class MagSizeMenuItem : public MenuItem       // Mag Size Menu Item 7
{
  private:
    
    
  public:

  void Increase()
  {
    if( CurrentValue == 0 )
      CurrentValue = 6;
    else if( CurrentValue < 100 )
      CurrentValue ++;
  }
  void Decrease()
  {
    if( CurrentValue == 6 ) 
    {
      CurrentValue = 0;
    }
    else if( CurrentValue > 6 )
    {
      CurrentValue --;
    }
  }

  void PrepareOutput()
  {
    if( CurrentValue == 0 )  
    {
      strcpy( Output, "  UP" );
    }
    else
    {
      sprintf( Output, "%4d", CurrentValue );
    }
  }
};

// ----------------------------------------------------------------------------
class TimingMenuItem : public MenuItem                // Timing Menu Item 8-11
{
  private:
    
    
  public:

  void Increase()
  {
    CurrentValue += 50;
    if( CurrentValue > 5000 )
      CurrentValue = 5000;
  }
  void Decrease()
  {
    CurrentValue -= 50;
    if( CurrentValue < 0 )
      CurrentValue = 0;
  }
  void PrepareOutput()
  {
    sprintf( Output, "%4d", CurrentValue );
  }
};

// ----------------------------------------------------------------------------
class TimingMenuItemFine : public MenuItem                // Timing Menu Item for fine adjustment of Ramp Up 
{
  private:
    
    
  public:

  void Increase()
  {
    CurrentValue += 10;
    if( CurrentValue > 5000 )
      CurrentValue = 5000;
  }
  void Decrease()
  {
    CurrentValue -= 10;
    if( CurrentValue < 0 )
      CurrentValue = 0;
  }
  void PrepareOutput()
  {
    sprintf( Output, "%4d", CurrentValue );
  }
};

//-------------------------------------------------------------------------------------------
class SolenoidTimingMenuItem : public MenuItem       //Solenoide Timing Menu Items 12, 13, 14
{
  private:
    
    
  public:

  void Increase()
  {
    if( CurrentValue == 0 )
      CurrentValue = 1;
    else if( CurrentValue < 1000 )
      CurrentValue ++;
  }
  void Decrease()
  {
    if( CurrentValue == 0 ) 
    {
      CurrentValue = 0;
    }
    else if( CurrentValue > 0 )
    {
      CurrentValue --;
    }
  }

  void PrepareOutput()
  {
    if( CurrentValue == 0 )  
    {
      strcpy( Output, "  UP" );
    }
    else
    {
      sprintf( Output, "%4d", CurrentValue );
    }
  }
};

// --------------------------------------------------------------------------------
class BatteryMenuItem : public MenuItem     //Battery type, Menu Item 15
{
  private:
    
    
  public:

  void Increase()
  {
    CurrentValue ++;
    if( CurrentValue > 4 )
      CurrentValue = 4;
  }
  void Decrease()
  {
    CurrentValue --;
    if( CurrentValue < 3 )
      CurrentValue = 3;
  }
  
   void PrepareOutput()
  {
    if( CurrentValue == 3 )
    {
      strcpy( Output, "  3S" );
    }
    if ( CurrentValue == 4 )
    {
      strcpy( Output, "  4S" ); 
    }
  }
};

// --------------------------------------------------------------------------------
class IdleSpeedMenuItem : public MenuItem                //Idle Speed  Menu Item 16
{
  private:
    
    
  public:

  void Increase()
  {
    if( CurrentValue == 100 )
      CurrentValue = 100;
    else if( CurrentValue < 100 )
      CurrentValue += 5;
  }
  void Decrease()
  {
    if( CurrentValue == 0 ) 
    {
      CurrentValue = 0;
    }
    else if( CurrentValue > 0 )
    {
      CurrentValue -= 5;
    }
  }

  void PrepareOutput()
  { 
      sprintf( Output, "%4d", CurrentValue );
  }
};

// --------------------------------------------------------------------------------
class ROF_RampTimeMenuItem : public MenuItem     //ROF RAMP Time on Auto, Menu Item 17
{
  private:
    
    
  public:
  
  void Increase()
  {
    if( CurrentValue == 0 )
      CurrentValue = 1;
    else if( CurrentValue < 1000 )
      CurrentValue ++;
  }
  void Decrease()
  {
    if( CurrentValue == 0 ) 
    {
      CurrentValue = 0;
    }
    else if( CurrentValue > 0 )
    {
      CurrentValue --;
    }
  }

  void PrepareOutput()
  { 
      sprintf( Output, "%4d", CurrentValue );
  }
};

// --------------------------------------------------------------------------------
class MagSwEnableMenuItem : public MenuItem     //AmmoCounterMenuItem, linked to 
{
  private:
    
    
  public:

  void Increase()
  {
    CurrentValue ++;
    if( CurrentValue > 1 )
      CurrentValue = 1;
  }
  void Decrease()
  {
    CurrentValue --;
    if( CurrentValue < 1 )
      CurrentValue = 0;
  }
  
   void PrepareOutput()
  {
    if( CurrentValue == 0 )
    {
      strcpy( Output, "  OFF" );
    }
    if ( CurrentValue == 1 )
    {
      strcpy( Output, "  ON" ); 
    }
  }

};

// ---------------------------------------------------------------------
class ExitMenuItem : public MenuItem
{
  private:
    char Buffer[0];
    
  public:

  void Increase()
  {
  }
  void Decrease()
  {
  }
  void PrepareOutput()
  {
  }
};

// -------------------------------------------------------------------------------------------

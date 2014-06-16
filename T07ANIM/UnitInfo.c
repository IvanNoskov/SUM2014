#include "anim.h"
#include "stdio.h"

/*--- INFO Display unit ---*/

/* INFODisplay unit
 * render function
 * base unit render arguments */
static VOID INFODisplayUnitRender( in1UNIT *Unit, in1ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);

}

/* INFODisplay unit
 * Response function
 * base unit render arguments */
static VOID INFODisplayUnitResponse( in1UNIT *Unit, in1ANIM *Ani )
{ 
  if (Ani->KeysClick[VK_ESCAPE])
    DestroyWindow( Ani->hWnd );
  if (Ani->KeysClick['F'])
    IN1_AnimFlipFullScreen( );
  if (Ani->KeysClick['P'])
    IN1_AnimSetPause( !Ani->IsPause );
}


/* INFO Display unit
 * creation function */
in1UNIT * IN1_INFODisplayUnitCreate( VOID )
{
  /* INFODisplay unit cpeation pointer */
  in1UNIT *Unit;

  /* getting memory | defolte unit model for INFODisplay unit */
  if ((Unit = IN1_AnimUnitCreate(sizeof(in1UNIT))) == NULL)
  /* no awailabe memory | INFODisplay unit model ERROR */
    return NULL;
  /* INFODisplay unit buse function initialization */
  Unit->Render = (VOID *)INFODisplayUnitRender;
  Unit->Response = (VOID *)INFODisplayUnitResponse;
  /* ready INFODisplay unit pointer return */
  return Unit;
}


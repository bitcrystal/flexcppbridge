// ActionScript file
import flash.events.MouseEvent;

import mx.collections.ArrayCollection;
import mx.controls.Alert;
import mx.controls.DataGrid;

private var emptygrid:DataGrid = null;
	 
     [Bindable]
     public var expenses:ArrayCollection = new ArrayCollection([
        {Expense:"Taxes", Amount:2000},
        {Expense:"Rent", Amount:1000},
        {Expense:"Bills", Amount:600},
        {Expense:"Car", Amount:450},
        {Expense:"Gas", Amount:200},
        {Expense:"Food", Amount:250}
     ]);
     
     [Bindable]
     public var explodingArray:Array = [0,0,0,0,0,0]
	
	 private function explodeSlice(event:Event):void{
	 	explodingArray = [0,0,0,0,0,0];
 		explodingArray[event.target.selectedChartItem.index]=0.1;
	 	pie1.series[0].perWedgeExplodeRadius = explodingArray;
	 	expenses.refresh();
	 	
	 	
	 }

public function testAlert(str:String, str1:String, obj1:Object, bidge:FlexVCBridge):FlexVCBridge
{
	Alert.show(str + " : " + str1 + " : " + obj1["Custom"] + " : " + bidge.width);
	
	return this;
}

public function testAlert1(str:String, str1:String, obj1:Object, bidge:FlexVCBridge, callbackfn:Function):FlexVCBridge
{
	Alert.show(str + " : " + str1 + " : " + obj1["Custom"] + " : " + bidge.width);
	var newobj:Object = new Object();
	
	newobj["testparam"] = "testval";
	callbackfn.call(this, newobj);	

	return bidge;
}

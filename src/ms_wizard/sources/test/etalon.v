//module: mod_name2
//mod desc2
module mod_name2( test1, test2, newName1, test11, test22, newName13);
//constants
	//state1
	parameter state1 = 2'b0;
	//state2
	parameter state2 = 2'b1;
	//state3
	parameter state3 = 2'b10;
	//state4
	parameter state4 = 2'b11;
//inputs
	input test1;
	wire test1;
	input test2;
	wire[2:0] test2;
	input newName1;
	wire newName1;
//outputs
	output test11;
	wire test11;
	output test22;
	reg test22;
	output newName13;
	wire newName13;
//temporary variables
	reg _test11;
	reg _test22;
	reg _newName13;

	reg[1:0] state;

	//Inicialization
	initial
	begin
		_test11 <= 01;
		_test22 <= 01;
		_newName13 <= 01;
		state <= state1;
		$monitor("Current state: state=%h", state);
	end


	//Enter by clock
	always @(posedge CLK)
	begin
		if(RST)
		begin
			state = state1;
		end else
		case(state)
			//state1
			state1:
			begin
				$display("mod_name2: Enter to state=state1");
				//TODO: some code...
				//transfer to next state
				if(rel1)
				begin
					$display("mod_name2: transfer to next state=state2");
					state = state2;
				end
				//control by outputs
				if(relationname1)
				begin
					$display("mod_name2: control by outputs: test11=1");
					_test11 = 1;
				end
				if(relationname2)
				begin
					$display("mod_name2: control by outputs: test22=2");
					_test22 = 2;
				end
			end
			//state2
			state2:
			begin
				$display("mod_name2: Enter to state=state2");
				//TODO: some code...
				//transfer to next state
				if(rel2)
				begin
					$display("mod_name2: transfer to next state=state3");
					state = state3;
				end
				//control by outputs
				if(relationname3)
				begin
					$display("mod_name2: control by outputs: test22=3");
					_test22 = 3;
				end
			end
			//state3
			state3:
			begin
				$display("mod_name2: Enter to state=state3");
				//TODO: some code...
				//transfer to next state
				if(rel3)
				begin
					$display("mod_name2: transfer to next state=state4");
					state = state4;
				end
				//control by outputs
			end
			//state4
			state4:
			begin
				$display("mod_name2: Enter to state=state4");
				//TODO: some code...
				//transfer to next state
				if(rel4)
				begin
					$display("mod_name2: transfer to next state=state1");
					state = state1;
				end
				//control by outputs
			end
		endcase
	end

	assign test11 = _test11;
	assign test22 = _test22;
	assign newName13 = _newName13;
endmodule

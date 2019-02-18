package uk.ac.nott.cs.g53dia.library;

import java.awt.*;
import javax.swing.*;

/**
 * A simple user interface for watching an individual Tanker.
 *
 * @author Neil Madden.
 */
/*
 * Copyright (c) 2003 Stuart Reeves Copyright (c) 2003-2005 Neil Madden
 * (nem@cs.nott.ac.uk). Copyright (c) 2011 Julian Zappala (jxz@cs.nott.ac.uk).
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */
public class TankerViewer extends JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2810783821678793885L;
	TankerViewerIconFactory iconfactory;
	JLabel[][] cells;
	JLabel tstep, fuel, pos, waste, disposed, score;
	Tanker tank;
	final static int SIZE = (Tanker.VIEW_RANGE * 2) + 1, ICON_SIZE = 25, PSIZE = SIZE * ICON_SIZE;

	public TankerViewer(Tanker Tanker) {
		this(Tanker, new DefaultTankerViewerIconFactory());
	}

	public TankerViewer(Tanker Tanker, TankerViewerIconFactory fac) {
		this.tank = Tanker;
		this.iconfactory = fac;
		Container c = getContentPane();
		c.setLayout(new BorderLayout());

		// Create the cell viewer
		cells = new JLabel[SIZE][SIZE];
		JLayeredPane lp = new JLayeredPane();
		JPanel p = new JPanel(new GridLayout(SIZE, SIZE));
		p.setBackground(Color.WHITE);

		for (int y = 0; y < SIZE; y++) {
			for (int x = 0; x < SIZE; x++) {
				cells[x][y] = new JLabel();
				p.add(cells[x][y]);
			}
		}

		lp.add(p, new Integer(0));
		p.setBounds(0, 0, PSIZE, PSIZE);
		// Create a Tanker label
		JLabel Tankerlabel = new JLabel(iconfactory.getIconForTanker(Tanker));
		lp.add(Tankerlabel, new Integer(1)); // Add above the background
		lp.setSize(new Dimension(PSIZE, PSIZE));
		Tankerlabel.setBounds(PSIZE / 2 - ICON_SIZE / 2, PSIZE / 2 - ICON_SIZE / 2, ICON_SIZE, ICON_SIZE);
		c.add(lp, BorderLayout.CENTER);

		// Create some labels to show info about the Tanker and environment
		JPanel infop = new JPanel(new GridLayout(3, 4));
		infop.add(new JLabel("Timestep:"));
		tstep = new JLabel("0");
		infop.add(tstep);
		infop.add(new JLabel("Fuel:"));
		fuel = new JLabel("200");
		infop.add(fuel);
		infop.add(new JLabel("Position:"));
		pos = new JLabel("(0,0)");
		infop.add(pos);
		infop.add(new JLabel("Waste:"));
		waste = new JLabel("0");
		infop.add(waste);
		infop.add(new JLabel("Disposed:"));
		disposed = new JLabel("0");
		infop.add(disposed);
		infop.add(new JLabel("Score:"));
		score = new JLabel("0");
		infop.add(score);

		c.add(infop, BorderLayout.SOUTH);
		// infop.setPreferredSize(new Dimension(200,100));

		setSize(PSIZE, PSIZE + 50);
		setTitle("Tanker Viewer");
		setVisible(true);
	}

	public void setTanker(Tanker t) {
		this.tank = t;
	}

	public void tick(Environment env) {
		Cell[][] view = env.getView(tank.getPosition(), Tanker.VIEW_RANGE);
		pos.setText(tank.getPosition().toString());
		tstep.setText(new String("" + env.getTimestep()));
		waste.setText(new String("" + tank.wasteLevel));
		fuel.setText(new String("" + tank.getFuelLevel()));
		disposed.setText("" + tank.wasteDisposed);
		score.setText("" + tank.getScore());
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE; y++) {
				Icon cur = iconfactory.getIconForCell(view[x][y]);
				cells[x][y].setIcon(cur);
			}
		}
	}
}
